// CExtCharDepot.cpp — 角色仓库系统完整实现
#include "Character/CExtCharDepot.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/Unit.h"
#include "Config/CfgData.h"
#include "Other/CVip.h"
#include "Other/StaticObj.h"
#include "Other/CEquipManager.h"
#include "Other/CItemHelper.h"
#include "Other/CChrDepot.h"
#include "Network/NetPacket.h"
#include "Answer/Singleton.h"
#include "Database/PlayerDBData.h"
#include "Common/CommonTypes.h"
#include <algorithm>
#include <cstring>
#include <cstdio>

// ============================================================================
// 排序比较器 (free function)
// ============================================================================
bool sortItem(const MemChrBag& left, const MemChrBag& right)
{
    if (left.itemClass < right.itemClass)
        return true;
    if (left.itemClass == right.itemClass)
        return left.itemId < right.itemId;
    return false;
}

// ============================================================================
// 额外 ICR/GCR 常量 (仓库专用)
// ============================================================================
static const int32_t ICR_DEPOT_GET       = 2000;  // 仓库取出
static const int32_t ICR_DEPOT_SAVE      = 2001;  // 仓库存入
static const int32_t GCR_DEPOT_GET       = 2000;  // 仓库货币取出
static const int32_t GCR_DEPOT_SLOT_OPEN = 2001;  // 仓库开格

// ============================================================================
// 生命周期
// ============================================================================
CExtCharDepot::CExtCharDepot()
{
    m_lstDirty.clear();
    OnCleanUp();
}

CExtCharDepot::~CExtCharDepot()
{
    m_lstDirty.clear();
}

void CExtCharDepot::OnCleanUp()
{
    memset(m_DepotData, 0, sizeof(m_DepotData));
    memset(&m_nullobj, 0, sizeof(m_nullobj));
    memset(m_aCurrency, 0, sizeof(m_aCurrency));
    m_lstDirty.clear();
    m_NeedSendCurrency = false;
    m_NeedSendDirty    = false;
    m_OpenCount        = 0;
}

void CExtCharDepot::OnUpdate(int64_t curTick)
{
    CheckDirty();
    CheckCurrency();
}

void CExtCharDepot::OnDaySwitch(int32_t nDiffDays)
{
    // 目前无每日切换逻辑
}

void CExtCharDepot::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_OpenCount = dbData->gambleDepot.OpneCount;
    m_Password  = dbData->gambleDepot.Password;
    m_SendPassword = dbData->gambleDepot.SecondPassword;
    memcpy(m_DepotData, dbData->gambleDepot.gambleDepot, sizeof(m_DepotData));
    memcpy(m_aCurrency, dbData->gambleDepot.Currency, sizeof(m_aCurrency));
}

void CExtCharDepot::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->gambleDepot.OpneCount = m_OpenCount;
    dbData->gambleDepot.Password  = m_Password;
    dbData->gambleDepot.SecondPassword = m_SendPassword;
    memcpy(dbData->gambleDepot.gambleDepot, m_DepotData, sizeof(dbData->gambleDepot.gambleDepot));
    memcpy(dbData->gambleDepot.Currency, m_aCurrency, sizeof(dbData->gambleDepot.Currency));
}

void CExtCharDepot::GetInterestsProtocol(ProcIdList* procList)
{
    uint16_t procs[] = {
        401, 402, 403, 404, 405, 406, 407, 408, 409, 410,
        503, 504, 505, 506
    };
    for (uint16_t p : procs)
        procList->push_back(p);
}

int32_t CExtCharDepot::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    switch (nProcId)
    {
    case 401: return OnGetDepotItem(inPacket);
    case 402: return OnSaveItemToDepot(inPacket);
    case 403: return OnGetDepotCurrency(inPacket);
    case 404: return OnSaveDepotCurrency(inPacket);
    case 405: return OnNewSortDepot(inPacket);
    case 406: return OnOpenDepotSlot(inPacket);
    case 407: return OnSetPassword(inPacket);
    case 408: return OnEnterPassword(inPacket);
    case 409: return OnModifyPassword(inPacket);
    case 410: return OnCancelPassword(inPacket);
    case 503: return OnSetSecondPassword(inPacket);
    case 504: return OnEnterSecondPassword(inPacket);
    case 505: return OnModifySecondPassword(inPacket);
    case 506: return OnCancelSecondPassword(inPacket);
    default:  return 0;
    }
}

// ============================================================================
// 核心查询
// ============================================================================
int32_t CExtCharDepot::GetDepotSize() const
{
    return BASE_SLOTS + m_OpenCount;
}

const MemChrBag* CExtCharDepot::GetSlotData(int32_t slot) const
{
    if (!IsSlotValid(slot))
        return &m_nullobj;
    return &m_DepotData[slot];
}

bool CExtCharDepot::IsSlotValid(int32_t slot) const
{
    return slot >= 0 && slot < GetDepotSize();
}

int32_t CExtCharDepot::GetItemCount(int32_t nId, int8_t nClass) const
{
    int32_t nCount = 0;
    int32_t nSize  = GetDepotSize();
    for (int32_t i = 0; i < nSize; ++i)
    {
        if (m_DepotData[i].itemId == nId && m_DepotData[i].itemClass == nClass)
            nCount += m_DepotData[i].itemCount;
    }
    return nCount;
}

int32_t CExtCharDepot::GetPageBySlot(int32_t slot) const
{
    return slot / BASE_SLOTS + 1;
}

// ============================================================================
// 槽位操作
// ============================================================================
bool CExtCharDepot::setSlotData(int32_t slot, const MemChrBag* slotData)
{
    if (!IsSlotValid(slot))
        return false;

    if (slotData->itemCount <= 0)
        m_DepotData[slot] = m_nullobj;
    else
        m_DepotData[slot] = *slotData;

    AddDirty(slot);
    return true;
}

bool CExtCharDepot::RemoveItem(int32_t slot)
{
    if (!IsSlotValid(slot))
        return false;
    setSlotData(slot, &m_nullobj);
    return true;
}

bool CExtCharDepot::AddItem(const MemChrBag* item)
{
    // 先尝试叠加到已有物品
    int32_t nSize = GetDepotSize();
    for (int32_t i = 0; i < nSize; ++i)
    {
        if (compairSlot(&m_DepotData[i], item))
        {
            m_DepotData[i].itemCount += item->itemCount;
            AddDirty(i);
            return true;
        }
    }

    // 找空位
    for (int32_t i = 0; i < nSize; ++i)
    {
        if (m_DepotData[i].itemCount <= 0)
        {
            m_DepotData[i] = *item;
            AddDirty(i);
            return true;
        }
    }

    return false; // 仓库已满
}

void CExtCharDepot::CleanBag()
{
    int32_t nSize = GetDepotSize();
    for (int32_t i = 0; i < nSize && IsSlotValid(i); ++i)
    {
        if (m_DepotData[i].itemId > 0)
        {
            if (m_DepotData[i].itemClass == 2 && m_DepotData[i].srcId > 0)
            {
                int64_t srcId = (int64_t)m_DepotData[i].srcId;
                int8_t connId  = Player::getConnId(m_pPlayer);
                CEquipManager* mgr = Answer::Singleton<CEquipManager>::instance();
                CEquipManager::DeleteMemEquip(mgr, connId, srcId, 0);
            }
            m_DepotData[i] = m_nullobj;
            AddDirty(i);
        }
    }
}

// ============================================================================
// 货币
// ============================================================================
int64_t CExtCharDepot::GetCurrency(int32_t nType) const
{
    if (nType >= 0 && nType < CURRENCY_TYPE_COUNT)
        return m_aCurrency[nType];
    return 0;
}

void CExtCharDepot::SendDepotCurrency()
{
    if (!m_pPlayer) return;

    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2C9Bu);
    if (!packet) return;

    uint32_t oldOffset = Answer::NetPacket::getWOffset(packet);
    int8_t nCount = 0;
    Answer::NetPacket::writeInt8(packet, 0);

    for (int8_t i = 0; i < CURRENCY_TYPE_COUNT; ++i)
    {
        if (m_aCurrency[i] > 0)
        {
            Answer::NetPacket::writeInt8(packet, i);
            Answer::NetPacket::writeInt64(packet, m_aCurrency[i]);
            ++nCount;
        }
    }

    uint32_t newOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, oldOffset);
    Answer::NetPacket::writeInt8(packet, nCount);
    Answer::NetPacket::setWOffset(packet, newOffset);

    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);

    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t  connId2   = Player::getConnId(m_pPlayer);
    GameService* gs2  = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

void CExtCharDepot::CheckCurrency()
{
    if (m_NeedSendCurrency)
    {
        SendDepotCurrency();
        m_NeedSendCurrency = false;
    }
}

// ============================================================================
// 脏标记
// ============================================================================
void CExtCharDepot::AddDirty(int32_t nSlot)
{
    // 检查是否已在脏列表中
    for (auto it = m_lstDirty.begin(); it != m_lstDirty.end(); ++it)
    {
        if (*it == nSlot)
            return;
    }
    m_lstDirty.push_back(nSlot);
    m_NeedSendDirty = true;
}

void CExtCharDepot::CheckDirty()
{
    if (m_NeedSendDirty && SendDirty())
    {
        ClearDirty();
        m_NeedSendDirty = false;
    }
}

bool CExtCharDepot::SendDirty()
{
    if (!m_pPlayer || m_lstDirty.empty())
        return false;

    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2DB7u);
    if (!packet) return false;

    Answer::NetPacket::writeInt8(packet, 2);

    uint32_t nOffSet = Answer::NetPacket::getWOffset(packet);
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);

    for (auto& nSlot : m_lstDirty)
    {
        const MemChrBag* tSlot = GetSlotData(nSlot);
        Answer::NetPacket::writeInt32(packet, nSlot);
        Answer::NetPacket::writeInt32(packet, tSlot->itemId);
        Answer::NetPacket::writeInt8(packet, tSlot->itemClass);
        Answer::NetPacket::writeInt32(packet, tSlot->itemCount);
        Answer::NetPacket::writeInt8(packet, tSlot->bind);
        Answer::NetPacket::writeInt32(packet, tSlot->endTime);
        Answer::NetPacket::writeInt64(packet, tSlot->srcId);
        ++nCount;
    }

    uint32_t endOffSet = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nOffSet);
    Answer::NetPacket::writeInt32(packet, nCount);
    Answer::NetPacket::setWOffset(packet, endOffSet);

    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);

    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t  connId2   = Player::getConnId(m_pPlayer);
    GameService* gs2  = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
    return true;
}

void CExtCharDepot::ClearDirty()
{
    m_lstDirty.clear();
}

// ============================================================================
// 网络发送
// ============================================================================
void CExtCharDepot::SendDepotItem()
{
    if (!m_pPlayer) return;

    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2C9Du);
    if (!packet) return;

    uint32_t nOffSet = Answer::NetPacket::getWOffset(packet);
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);

    int32_t nDepotSize = GetDepotSize();
    for (int32_t slot = 0; slot < nDepotSize; ++slot)
    {
        if (m_DepotData[slot].itemCount > 0)
        {
            Answer::NetPacket::writeInt32(packet, slot);
            Answer::NetPacket::writeInt32(packet, m_DepotData[slot].itemId);
            Answer::NetPacket::writeInt8(packet, m_DepotData[slot].itemClass);
            Answer::NetPacket::writeInt32(packet, m_DepotData[slot].itemCount);
            Answer::NetPacket::writeInt8(packet, m_DepotData[slot].bind);
            Answer::NetPacket::writeInt32(packet, m_DepotData[slot].endTime);
            Answer::NetPacket::writeInt64(packet, m_DepotData[slot].srcId);
            ++nCount;
        }
    }

    uint32_t endOffSet = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nOffSet);
    Answer::NetPacket::writeInt32(packet, nCount);
    Answer::NetPacket::setWOffset(packet, endOffSet);

    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);

    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t  connId2   = Player::getConnId(m_pPlayer);
    GameService* gs2  = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

void CExtCharDepot::SendDepotInfo()
{
    if (!m_pPlayer) return;

    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2C9Cu);
    if (!packet) return;

    int32_t maxSlot = GetDepotSize() - 1;
    Answer::NetPacket::writeInt32(packet, maxSlot);

    if (m_Password.empty())
        Answer::NetPacket::writeInt8(packet, 0);
    else
        Answer::NetPacket::writeInt8(packet, 1);

    if (m_SendPassword.empty())
        Answer::NetPacket::writeInt8(packet, 0);
    else
        Answer::NetPacket::writeInt8(packet, 1);

    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);

    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t  connId2   = Player::getConnId(m_pPlayer);
    GameService* gs2  = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

// ============================================================================
// 访问控制
// ============================================================================
bool CExtCharDepot::CanOperateDepot()
{
    if (!m_pPlayer)
        return false;

    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (!CVip::GetVipFlg(pVip, 2) && StaticObj::getMapId(m_pPlayer) != 50001)
        return false;

    if (m_Password.empty())
        return true;

    return Player::getRecord(m_pPlayer, 2) == 1;
}

bool CExtCharDepot::IsEnterSecondPasswordOperate()
{
    if (!m_pPlayer)
        return false;

    if (m_SendPassword.empty())
        return true;

    return Player::getRecord(m_pPlayer, 9) == 1;
}

// ============================================================================
// 网络协议处理器
// ============================================================================
int32_t CExtCharDepot::OnGetDepotItem(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (!CanOperateDepot()) return 2;

    int32_t DepotSlot = Answer::NetPacket::readInt32(inPacket);
    int32_t BagSlot   = Answer::NetPacket::readInt32(inPacket);

    if (!IsSlotValid(DepotSlot)) return 2;

    MemChrBag SlotData = *GetSlotData(DepotSlot);
    if (SlotData.itemId <= 0 || SlotData.itemCount <= 0) return 2;

    if (BagSlot < 0)
    {
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!Bag->AddItem(&SlotData, (ITEM_CHANGE_REASON)ICR_DEPOT_GET))
        {
            uint16_t Proc = Answer::NetPacket::getProc(inPacket);
            int16_t  gateIndex = Player::getGateIndex(m_pPlayer);
            int8_t   connId    = Player::getConnId(m_pPlayer);
            GameService* gs = Answer::Singleton<GameService>::instance();
            GameService::replyfailure(gs, connId, gateIndex, Proc, 10016, 0);
            return 10016;
        }
    }
    else
    {
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!Bag->IsSlotValid(BagSlot)) return 2;

        MemChrBag BagItem;
        Player::getBagSlotData(&BagItem, m_pPlayer, BagSlot);
        if (BagItem.itemId && BagItem.itemCount) return 2;

        Bag->SetSlotData(BagSlot, &SlotData, (ITEM_CHANGE_REASON)ICR_DEPOT_GET, 0);
    }

    RemoveItem(DepotSlot);
    return 0;
}

int32_t CExtCharDepot::OnSaveItemToDepot(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (!CanOperateDepot()) return 2;

    int32_t BagSlot   = Answer::NetPacket::readInt32(inPacket);
    int32_t DepotSlot = Answer::NetPacket::readInt32(inPacket);

    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    int32_t Page = CVip::GetStorePage(pVip);

    if (GetPageBySlot(DepotSlot) > Page) return 2;

    MemChrBag BagItem;
    Player::getBagSlotData(&BagItem, m_pPlayer, BagSlot);
    if (!BagItem.itemId || !BagItem.itemCount) return 2;

    if (DepotSlot < 0)
    {
        if (!AddItem(&BagItem)) return 2;
    }
    else
    {
        const MemChrBag* SlotData = GetSlotData(DepotSlot);
        if (SlotData->itemId && SlotData->itemCount) return 2;
        setSlotData(DepotSlot, &BagItem);
    }

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    Bag->SetSlotData(BagSlot, &m_nullobj, (ITEM_CHANGE_REASON)ICR_DEPOT_SAVE, 0);
    return 0;
}

int32_t CExtCharDepot::OnGetDepotCurrency(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (!CanOperateDepot()) return 2;

    int8_t nType = Answer::NetPacket::readInt8(inPacket);
    int32_t Value = Answer::NetPacket::readInt32(inPacket);

    if ((uint8_t)nType >= CURRENCY_TYPE_COUNT) return 2;
    if (Value <= 0) return 2;
    if (GetCurrency(nType) < Value) return 2;

    m_aCurrency[nType] -= Value;
    Player::AddCurrency(m_pPlayer, (CURRENCY_TYPE)nType, Value, (CURRENCY_CHANGE_REASON)GCR_DEPOT_GET, 0);
    m_NeedSendCurrency = true;
    return 0;
}

int32_t CExtCharDepot::OnSaveDepotCurrency(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 2;
    if (!CanOperateDepot()) return 2;

    int8_t nType = Answer::NetPacket::readInt8(inPacket);
    int32_t Value = Answer::NetPacket::readInt32(inPacket);

    if ((uint8_t)nType >= CURRENCY_TYPE_COUNT) return 2;
    if (Value <= 0) return 2;
    if (Player::GetCurrency(m_pPlayer, (CURRENCY_TYPE)nType) < Value) return 2;

    m_aCurrency[nType] += Value;
    Player::DecCurrency(m_pPlayer, (CURRENCY_TYPE)nType, Value, (CURRENCY_CHANGE_REASON)GCR_DEPOT_GET, 0);
    m_NeedSendCurrency = true;
    return 0;
}

int32_t CExtCharDepot::OnNewSortDepot(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (!CanOperateDepot()) return 2;

    int32_t Page    = Answer::NetPacket::readInt32(inPacket);
    int32_t MinSlot = BASE_SLOTS * Page;
    int32_t MaxSlot = BASE_SLOTS * (Page + 1) - 1;

    if (MinSlot < 0 || MaxSlot > (MAX_SLOTS - 1)) return 10002;
    if (MinSlot >= MaxSlot) return 10002;

    // 收集该页所有物品，合并可叠加的
    MemChrBagVector items;
    for (int32_t i = MinSlot; i <= MaxSlot; ++i)
    {
        const MemChrBag* slotdata = GetSlotData(i);
        if (slotdata->itemCount > 0)
        {
            bool bFind = false;
            for (auto& it : items)
            {
                if (compairSlot(&it, slotdata))
                {
                    it.itemCount += slotdata->itemCount;
                    bFind = true;
                    break;
                }
            }
            if (!bFind)
                items.push_back(*slotdata);
        }
    }

    if (items.size() <= (size_t)(BASE_SLOTS))
    {
        std::sort(items.begin(), items.end(), sortItem);

        int32_t slot = MinSlot;
        for (size_t idx = 0; idx < items.size() && slot <= MaxSlot; ++idx)
        {
            MemChrBag& bagSlot = items[idx];
            int32_t overlay = CfgData::getOverlay(
                Answer::Singleton<CfgData>::instance(),
                bagSlot.itemId, bagSlot.itemClass);

            if (overlay > 0)
            {
                while (bagSlot.itemCount > overlay && slot <= MaxSlot)
                {
                    MemChrBag tSlot = bagSlot;
                    tSlot.itemCount = overlay;
                    bagSlot.itemCount -= overlay;
                    setSlotData(slot++, &tSlot);
                }
                if (bagSlot.itemCount > 0 && slot <= MaxSlot)
                    setSlotData(slot++, &bagSlot);
            }
        }
        while (slot <= MaxSlot)
            setSlotData(slot++, &m_nullobj);

        return 0;
    }

    return 10002;
}

int32_t CExtCharDepot::OnSortDepot(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (!CanOperateDepot()) return 2;

    // 收集全部物品
    MemChrBagVector items;
    int32_t nDepotSize = GetDepotSize();
    for (int32_t i = 0; i < nDepotSize; ++i)
    {
        const MemChrBag* slotdata = GetSlotData(i);
        if (slotdata->itemCount > 0)
        {
            bool bFind = false;
            for (auto& it : items)
            {
                if (compairSlot(&it, slotdata))
                {
                    it.itemCount += slotdata->itemCount;
                    bFind = true;
                    break;
                }
            }
            if (!bFind)
                items.push_back(*slotdata);
        }
    }

    std::sort(items.begin(), items.end(), sortItem);

    int32_t slot = 0;
    for (size_t idx = 0; idx < items.size() && slot < nDepotSize; ++idx)
    {
        MemChrBag& bagSlot = items[idx];
        int32_t overlay = CfgData::getOverlay(
            Answer::Singleton<CfgData>::instance(),
            bagSlot.itemId, bagSlot.itemClass);

        if (overlay > 0)
        {
            while (bagSlot.itemCount > overlay && slot < nDepotSize)
            {
                MemChrBag tSlot = bagSlot;
                tSlot.itemCount = overlay;
                bagSlot.itemCount -= overlay;
                setSlotData(slot++, &tSlot);
            }
            if (bagSlot.itemCount > 0 && slot < nDepotSize)
                setSlotData(slot++, &bagSlot);
        }
    }
    while (slot < nDepotSize)
        setSlotData(slot++, &m_nullobj);

    return 0;
}

int32_t CExtCharDepot::OnOpenDepotSlot(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (!CanOperateDepot()) return 2;

    int32_t slot = Answer::NetPacket::readInt32(inPacket);
    if ((uint32_t)slot >= MAX_SLOTS) return 10002;
    if (IsSlotValid(slot)) return 10002;

    int32_t CostValues = 0;
    int32_t OpenCount = slot - GetDepotSize() + 1;

    for (int32_t i = GetDepotSize(); i <= slot; ++i)
        CostValues += OpenSlotCostGold(i);

    if (CostValues <= 0) return 10002;

    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, CostValues,
            (CURRENCY_CHANGE_REASON)GCR_DEPOT_SLOT_OPEN, 0))
        return 10002;

    m_OpenCount += OpenCount;
    SendDepotInfo();
    return 0;
}

int32_t CExtCharDepot::OpenSlotCostGold(int32_t slot)
{
    if (IsSlotValid(slot))
        return 0;
    return 10 * ((slot - BASE_SLOTS) / 5 + 1);
}

// ============================================================================
// 密码处理器
// ============================================================================
int32_t CExtCharDepot::OnSetPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (!m_Password.empty()) return 10002;

    std::string OnePassword;
    std::string TwoPassword;
    Answer::NetPacket::readUTF8(inPacket, &OnePassword);
    Answer::NetPacket::readUTF8(inPacket, &TwoPassword);

    if (OnePassword != TwoPassword)
    {
        Player::TiShiInfo(m_pPlayer, 16, 0);
        return 10002;
    }

    m_Password = OnePassword;
    Player::TiShiInfo(m_pPlayer, 14, 0);
    Player::updateRecord(m_pPlayer, 2, 1);
    SendDepotInfo();
    return 0;
}

int32_t CExtCharDepot::OnEnterPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_Password.empty()) return 10002;

    std::string Password;
    Answer::NetPacket::readUTF8(inPacket, &Password);

    if (Password != m_Password)
    {
        Player::TiShiInfo(m_pPlayer, 15, 0);
        return 10002;
    }

    Player::updateRecord(m_pPlayer, 2, 1);

    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t  gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t   connId    = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, Proc, 0);
    return 0;
}

int32_t CExtCharDepot::OnModifyPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_Password.empty()) return 10002;

    std::string OldPassword;
    std::string OnePassword;
    std::string TwoPassword;
    Answer::NetPacket::readUTF8(inPacket, &OldPassword);
    Answer::NetPacket::readUTF8(inPacket, &OnePassword);
    Answer::NetPacket::readUTF8(inPacket, &TwoPassword);

    if (OldPassword != m_Password)
    {
        Player::TiShiInfo(m_pPlayer, 15, 0);
        return 10002;
    }

    if (OnePassword != TwoPassword)
    {
        Player::TiShiInfo(m_pPlayer, 16, 0);
        return 10002;
    }

    m_Password = OnePassword;
    Player::TiShiInfo(m_pPlayer, 17, 0);
    return 0;
}

int32_t CExtCharDepot::OnCancelPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_Password.empty()) return 10002;

    std::string OldPassword;
    Answer::NetPacket::readUTF8(inPacket, &OldPassword);

    if (OldPassword != m_Password)
    {
        Player::TiShiInfo(m_pPlayer, 15, 0);
        return 10002;
    }

    m_Password.clear();
    SendDepotInfo();
    return 0;
}

int32_t CExtCharDepot::OnSetSecondPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (!m_SendPassword.empty()) return 10002;

    std::string OnePassword;
    std::string TwoPassword;
    Answer::NetPacket::readUTF8(inPacket, &OnePassword);
    Answer::NetPacket::readUTF8(inPacket, &TwoPassword);

    if (OnePassword != TwoPassword)
    {
        Player::TiShiInfo(m_pPlayer, 16, 0);
        return 10002;
    }

    m_SendPassword = OnePassword;
    Player::TiShiInfo(m_pPlayer, 14, 0);
    Player::updateRecord(m_pPlayer, 9, 1);
    SendDepotInfo();
    return 0;
}

int32_t CExtCharDepot::OnEnterSecondPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_SendPassword.empty()) return 10002;

    std::string Password;
    Answer::NetPacket::readUTF8(inPacket, &Password);

    if (Password != m_SendPassword)
    {
        Player::TiShiInfo(m_pPlayer, 15, 0);
        return 10002;
    }

    Player::updateRecord(m_pPlayer, 9, 1);

    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t  gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t   connId    = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, Proc, 0);
    return 0;
}

int32_t CExtCharDepot::OnModifySecondPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_SendPassword.empty()) return 10002;

    std::string OldPassword;
    std::string OnePassword;
    std::string TwoPassword;
    Answer::NetPacket::readUTF8(inPacket, &OldPassword);
    Answer::NetPacket::readUTF8(inPacket, &OnePassword);
    Answer::NetPacket::readUTF8(inPacket, &TwoPassword);

    if (OldPassword != m_SendPassword)
    {
        Player::TiShiInfo(m_pPlayer, 15, 0);
        return 10002;
    }

    if (OnePassword != TwoPassword)
    {
        Player::TiShiInfo(m_pPlayer, 16, 0);
        return 10002;
    }

    m_SendPassword = OnePassword;
    Player::TiShiInfo(m_pPlayer, 17, 0);
    return 0;
}

int32_t CExtCharDepot::OnCancelSecondPassword(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_SendPassword.empty()) return 10002;

    std::string OldPassword;
    Answer::NetPacket::readUTF8(inPacket, &OldPassword);

    if (OldPassword != m_SendPassword)
    {
        Player::TiShiInfo(m_pPlayer, 15, 0);
        return 10002;
    }

    m_SendPassword.clear();
    SendDepotInfo();
    return 0;
}

// ============================================================================
// 比较器
// ============================================================================
bool CExtCharDepot::compairSlot(const MemChrBag* left, const MemChrBag* right) const
{
    if (left->itemId != right->itemId) return false;
    if (left->srcId != right->srcId) return false;
    if (left->itemClass != right->itemClass) return false;
    if (left->bind == right->bind)
        return left->endTime == right->endTime;
    return false;
}
