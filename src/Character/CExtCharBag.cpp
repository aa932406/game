// CExtCharBag.cpp
#include "CExtCharBag.h"
#include "Answer/Logger.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Config/CfgData.h"
#include "Game/Unit.h"
#include "Other/ItemEffectManager.h"
#include "Other/CItemHelper.h"
#include "Other/CEquipManager.h"
#include "Other/CChrDepot.h"
#include "Other/ChrTask.h"
#include "Other/MemEquip.h"
#include "Other/Log360.h"
#include "Other/StaticObj.h"
#include "Database/PlayerDBData.h"
#include "Network/NetPacket.h"
#include "Other/CVplan.h"
#include "Other/CWuHunShop.h"
#include "Other/CXingMai.h"
#include "Other/CYaBiao.h"
#include "Other/CXiHongShi.h"
#include "Other/CLingZhu.h"
#include "Other/CXuNiBi.h"
#include "Other/CExtOperateLimit.h"
#include "Other/CXinMo.h"
#include "Other/Answer.h"
#include "Database/DBService.h"
#include "Answer/Singleton.h"
#include "Common/CommonTypes.h"
#include <cstring>
#include <algorithm>
#include <cstdio>

// ============================================================================
// 排序比较函数
// ============================================================================
bool CExtCharBag::GreaterItem(const MemChrBag& left, const MemChrBag& right)
{
    if (left.itemClass != right.itemClass)
        return left.itemClass < right.itemClass;
    if (left.itemId != right.itemId)
        return left.itemId < right.itemId;
    if (left.bind != right.bind)
        return left.bind < right.bind;
    return left.endTime < right.endTime;
}

// ============================================================================
// 生命周期方法
// ============================================================================

CExtCharBag::CExtCharBag()
{
    reset();
}

CExtCharBag::~CExtCharBag()
{
    m_lstItemChangeLog.clear();
    m_lstDirty.clear();
    m_lstWaitCreate.clear();
    m_ItemLimit.clear();
}

void CExtCharBag::OnCleanUp()
{
    reset();
    m_pPlayer = nullptr;
}

void CExtCharBag::reset()
{
    memset(m_bagData, 0, sizeof(m_bagData));
    memset(m_sellItemInfo, 0, sizeof(m_sellItemInfo));
    memset(&m_nullobj, 0, sizeof(m_nullobj));
    memset(m_lastItemTick, 0, sizeof(m_lastItemTick));
    memset(m_bagBackUp, 0, sizeof(m_bagBackUp));
    m_lstItemChangeLog.clear();
    m_lstDirty.clear();
    m_lstWaitCreate.clear();
    m_ItemLimit.clear();
}

void CExtCharBag::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        memcpy(m_bagData, dbData->bagData.bagData, sizeof(m_bagData));
        m_ItemLimit = dbData->bagData.m_ItemLimit;
    }
}

void CExtCharBag::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        memcpy(dbData->bagData.bagData, m_bagData, sizeof(dbData->bagData.bagData));
        dbData->bagData.m_ItemLimit = m_ItemLimit;
    }
}

void CExtCharBag::OnUpdate(int64_t curTick)
{
    checkCreate();
    checkDirty();
}

void CExtCharBag::GetInterestsProtocol(ProcIdList* procList)
{
    if (!procList) return;
    ProcId_t ids[] = {91, 92, 94, 95, 88, 89, 90, 481, 96, 20076, 657};
    for (int i = 0; i < 11; ++i)
        procList->push_back(ids[i]);
}

int32_t CExtCharBag::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 10002;
    if (nProcId == 94) return onSort(inPacket);
    if (nProcId == 481) return onUseMutiItem(inPacket);
    if (nProcId == 657) return onSelectItem(inPacket);
    if (nProcId == 20076) return onAddItem(inPacket);
    if (nProcId == 96) return onDiscardItem(inPacket);
    if (nProcId == 90) return onPatchUseItem(inPacket);
    if (nProcId == 91) return onMoveItem(inPacket);
    if (nProcId == 92) return onSplitItem(inPacket);
    if (nProcId == 89) return onUseItem(inPacket);
    return 0;
}

void CExtCharBag::OnDaySwitch(int32_t nDiffDays)
{
    m_ItemLimit.clear();
    SendLimitCount();
}

void CExtCharBag::cleanSellItem()
{
    for (int i = 0; i < SELL_SIZE; ++i)
    {
        if (m_sellItemInfo[i].srcId > 0 && m_sellItemInfo[i].itemClass == 2)
        {
            int64_t srcId = m_sellItemInfo[i].srcId;
            int8_t connId = Player::getConnId(m_pPlayer);
            CEquipManager* mgr = Answer::Singleton<CEquipManager>::instance();
            CEquipManager::DeleteMemEquip(mgr, connId, srcId, ICR_SELL);
        }
    }
    memset(m_sellItemInfo, 0, sizeof(m_sellItemInfo));
}

// ============================================================================
// 查询接口
// ============================================================================

int32_t CExtCharBag::GetBagSize() const
{
    return BAG_SIZE;
}

const MemChrBag* CExtCharBag::GetSlotData(int32_t slot) const
{
    if (!IsSlotValid(slot))
        return &m_nullobj;
    return &m_bagData[slot];
}

int32_t CExtCharBag::GetTypeItem(int32_t nType)
{
    int32_t bagSize = GetBagSize();
    for (int32_t i = 0; i < bagSize; ++i)
    {
        if (CItemHelper::GetItemType(m_bagData[i].itemId, m_bagData[i].itemClass) == nType)
            return i;
    }
    return -1;
}

int32_t CExtCharBag::GetItemCount(int32_t nId, int8_t nClass) const
{
    int32_t nCount = 0;
    int32_t bagSize = GetBagSize();
    for (int32_t i = 0; i < bagSize; ++i)
    {
        const MemChrBag* slotData = GetSlotData(i);
        if (!IsEmptySlot(slotData))
        {
            bool expired = false;
            if (slotData->endTime > 0 && m_pPlayer)
            {
                if (slotData->endTime <= Unit::getNow(m_pPlayer))
                    expired = true;
            }
            if (!expired && slotData->itemClass == nClass && slotData->itemId == nId)
                nCount += slotData->itemCount;
        }
    }
    return nCount;
}

int32_t CExtCharBag::GetItemCount(const Int32Vector* vSlot, int8_t nClass, int32_t nId) const
{
    int32_t nCount = 0;
    int32_t isize = (int32_t)vSlot->size();
    for (int32_t i = 0; i < isize; ++i)
    {
        int32_t idx = (*vSlot)[i];
        if (IsSlotValid(idx))
        {
            const MemChrBag* bagSlot = GetSlotData(idx);
            if (bagSlot->itemId == nId && bagSlot->itemClass == nClass)
            {
                bool expired = (bagSlot->endTime > 0 && m_pPlayer && bagSlot->endTime <= Unit::getNow(m_pPlayer));
                if (!expired)
                    nCount += bagSlot->itemCount;
            }
        }
    }
    return nCount;
}

bool CExtCharBag::HasItem(int32_t nId, int8_t nClass, int32_t nCount) const
{
    int32_t countHas = 0;
    int32_t bagSize = GetBagSize();
    for (int32_t slot = 0; slot < bagSize; ++slot)
    {
        const MemChrBag* slotData = GetSlotData(slot);
        if (slotData->itemCount > 0 && slotData->itemClass == nClass)
        {
            bool expired = false;
            if (slotData->endTime > 0 && m_pPlayer)
            {
                if (slotData->endTime <= Unit::getNow(m_pPlayer))
                    expired = true;
            }
            if (!expired && slotData->itemId == nId)
            {
                countHas += slotData->itemCount;
                if (countHas >= nCount)
                    return true;
            }
        }
    }
    return false;
}

bool CExtCharBag::HasItem(const ItemData* data) const
{
    return HasItem(data->m_nId, data->m_nClass, data->m_nCount);
}

int32_t CExtCharBag::HaveItemCount(const ItemData* data, bool IsBind, bool IsTimeLiness) const
{
    int32_t bagSize = GetBagSize();
    int32_t count = 0;
    for (int32_t i = 0; i < bagSize; ++i)
    {
        if (m_bagData[i].itemId == data->m_nId && m_bagData[i].itemClass == data->m_nClass)
        {
            if (IsBind && (m_bagData[i].bind == 1 || m_bagData[i].bind == 2))
            {
                if (IsTimeLiness)
                {
                    if (m_bagData[i].endTime <= Unit::getNow(m_pPlayer))
                        count += m_bagData[i].itemCount;
                }
                else if (!m_bagData[i].endTime)
                {
                    count += m_bagData[i].itemCount;
                }
            }
            else if (!IsBind && !m_bagData[i].bind)
            {
                if (IsTimeLiness)
                {
                    if (m_bagData[i].endTime <= Unit::getNow(m_pPlayer))
                        count += m_bagData[i].itemCount;
                }
                else if (!m_bagData[i].endTime)
                {
                    count += m_bagData[i].itemCount;
                }
            }
        }
    }
    return count;
}

int32_t CExtCharBag::GetFirstFreeSlot() const
{
    int32_t bagSize = GetBagSize();
    for (int32_t slot = 0; slot < bagSize; ++slot)
    {
        const MemChrBag* slotData = GetSlotData(slot);
        if (IsEmptySlot(slotData))
            return slot;
    }
    return -1;
}

int32_t CExtCharBag::GetFreeSlotCount() const
{
    int32_t count = 0;
    int32_t bagSize = GetBagSize();
    for (int32_t slot = 0; slot < bagSize; ++slot)
    {
        const MemChrBag* slotData = GetSlotData(slot);
        if (IsEmptySlot(slotData))
            ++count;
    }
    return count;
}

bool CExtCharBag::IsSlotValid(int32_t slot) const
{
    return slot >= 0 && slot < GetBagSize();
}

bool CExtCharBag::IsEmptySlot(const MemChrBag* slot) const
{
    return !slot || slot->itemId <= 0 || slot->itemCount <= 0;
}

int32_t CExtCharBag::GetItemType(const MemChrBag* item) const
{
    return CItemHelper::GetItemType(item->itemId, item->itemClass);
}

void CExtCharBag::GetItemSlot(Int32Vector* retstr, int8_t ItemClass, int32_t ItemId, int32_t ExcludeSlot) const
{
    retstr->clear();
    if (!m_pPlayer) return;
    for (int32_t i = 0; i < BAG_SIZE; ++i)
    {
        bool expired = (m_bagData[i].endTime > 0 && m_bagData[i].endTime < Unit::getNow(m_pPlayer));
        if (!expired
            && m_bagData[i].itemId == ItemId
            && m_bagData[i].itemClass == ItemClass
            && (ExcludeSlot < 0 || i != ExcludeSlot))
        {
            retstr->push_back(i);
        }
    }
}

void CExtCharBag::GetItemSlot(Int32Vector* retstr, int8_t ItemClass, int32_t ItemId, int32_t* Count, bool IsBind) const
{
    retstr->clear();
    if (!m_pPlayer) return;
    if (IsBind)
    {
        for (int32_t i = 0; i < BAG_SIZE; ++i)
        {
            bool expired = (m_bagData[i].endTime > 0 && m_bagData[i].endTime < Unit::getNow(m_pPlayer));
            if (!expired && (m_bagData[i].bind == 1 || m_bagData[i].bind == 2))
            {
                *Count += m_bagData[i].itemCount;
                retstr->push_back(i);
            }
        }
    }
    else
    {
        for (int32_t i = 0; i < BAG_SIZE; ++i)
        {
            bool expired = (m_bagData[i].endTime > 0 && m_bagData[i].endTime < Unit::getNow(m_pPlayer));
            if (!expired && !m_bagData[i].bind)
            {
                *Count += m_bagData[i].itemCount;
                retstr->push_back(i);
            }
        }
    }
}

// ============================================================================
// 限购系统
// ============================================================================

int32_t CExtCharBag::getLimitCount(int32_t index)
{
    auto it = m_ItemLimit.find(index);
    if (it != m_ItemLimit.end()) return it->second;
    return 0;
}

void CExtCharBag::addLimitCount(int32_t index, int32_t count)
{
    auto it = m_ItemLimit.find(index);
    if (it != m_ItemLimit.end())
        it->second += count;
    else
        m_ItemLimit[index] = count;
    SendLimitCount();
}

void CExtCharBag::SendLimitCount()
{
    if (!m_pPlayer) return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2DBA);
    if (!packet) return;
    int32_t count = (int32_t)m_ItemLimit.size();
    Answer::NetPacket::writeInt32(packet, count);
    for (auto& kv : m_ItemLimit)
    {
        Answer::NetPacket::writeInt32(packet, kv.first);
        Answer::NetPacket::writeInt32(packet, kv.second);
    }
    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t cid = Player::getConnId(m_pPlayer);
    GameService::sendPacketTo(gs, cid, gateIndex, packet);
}

// ============================================================================
// 发送到客户端
// ============================================================================

void CExtCharBag::SendBagItem()
{
    if (!m_pPlayer) return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2759);
    if (!packet) return;
    uint32_t nOffSet = Answer::NetPacket::getWOffset(packet);
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);
    int32_t bagSize = GetBagSize();
    for (int32_t slot = 0; slot < bagSize; ++slot)
    {
        const MemChrBag* tSlot = &m_bagData[slot];
        if (tSlot->itemCount > 0)
        {
            Answer::NetPacket::writeInt32(packet, slot);
            Answer::NetPacket::writeInt32(packet, tSlot->itemId);
            Answer::NetPacket::writeInt8(packet, tSlot->itemClass);
            Answer::NetPacket::writeInt32(packet, tSlot->itemCount);
            Answer::NetPacket::writeInt8(packet, tSlot->bind);
            Answer::NetPacket::writeInt32(packet, tSlot->endTime);
            Answer::NetPacket::writeInt64(packet, tSlot->srcId);
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
    int8_t cid = Player::getConnId(m_pPlayer);
    GameService::sendPacketTo(gs, cid, gateIndex, packet);
}

void CExtCharBag::SendBagSellItem()
{
    if (!m_pPlayer) return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2DB9);
    if (!packet) return;
    uint32_t nOffSet = Answer::NetPacket::getWOffset(packet);
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);
    for (int i = 0; i < SELL_SIZE; ++i)
    {
        const MemChrBag* tSlot = &m_sellItemInfo[i];
        if (tSlot->itemCount > 0)
        {
            Answer::NetPacket::writeInt32(packet, i);
            Answer::NetPacket::writeInt32(packet, tSlot->itemId);
            Answer::NetPacket::writeInt8(packet, tSlot->itemClass);
            Answer::NetPacket::writeInt32(packet, tSlot->itemCount);
            Answer::NetPacket::writeInt8(packet, tSlot->bind);
            Answer::NetPacket::writeInt32(packet, tSlot->endTime);
            Answer::NetPacket::writeInt64(packet, tSlot->srcId);
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
    int8_t cid = Player::getConnId(m_pPlayer);
    GameService::sendPacketTo(gs, cid, gateIndex, packet);
}

void CExtCharBag::sendGoldCashChange(int32_t type, int32_t addon, int32_t benefitType)
{
    if (!m_pPlayer) return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x275C);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, type);
    Answer::NetPacket::writeInt32(packet, addon);
    Answer::NetPacket::writeInt32(packet, benefitType);
    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t cid = Player::getConnId(m_pPlayer);
    GameService::sendPacketTo(gs, cid, gateIndex, packet);
}

void CExtCharBag::SaveDBData(void* dbData)
{
    if (dbData)
        memcpy(((PlayerDBData*)dbData)->bagData.bagData, m_bagData, sizeof(m_bagData));
}

void CExtCharBag::setSellItem(const MemChrBag* sellItem)
{
    if (m_sellItemInfo[SELL_SIZE - 1].srcId > 0 && m_sellItemInfo[SELL_SIZE - 1].itemClass == 2)
    {
        int64_t srcId = m_sellItemInfo[SELL_SIZE - 1].srcId;
        int8_t connId = Player::getConnId(m_pPlayer);
        CEquipManager* mgr = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::DeleteMemEquip(mgr, connId, srcId, ICR_SELL);
    }
    for (int i = SELL_SIZE - 1; i > 0; --i)
        m_sellItemInfo[i] = m_sellItemInfo[i - 1];
    m_sellItemInfo[0] = *sellItem;
}

void CExtCharBag::deleteSellItem(int32_t index)
{
    if (index < 0 || index >= SELL_SIZE) return;
    for (int i = index; i < SELL_SIZE - 1; ++i)
        m_sellItemInfo[i] = m_sellItemInfo[i + 1];
    memset(&m_sellItemInfo[SELL_SIZE - 1], 0, sizeof(MemChrBag));
}

// ============================================================================
// 槽位操作
// ============================================================================

void CExtCharBag::SetSlotData(int32_t slot, const MemChrBag* slotData, ITEM_CHANGE_REASON reason, int32_t count)
{
    setSlotData(slot, slotData, reason, count);
    saveItemChangeLog();
}

void CExtCharBag::setSlotData(int32_t slot, const MemChrBag* slotData, ITEM_CHANGE_REASON reason, int32_t count)
{
    if (!IsSlotValid(slot)) return;
    if (slotData->itemCount <= 0)
    {
        if (m_bagData[slot].itemId > 0)
            addItemChangeLog(reason, m_bagData[slot].itemId, m_bagData[slot].itemClass,
                m_bagData[slot].srcId, m_bagData[slot].itemCount, 2, 0);
        m_bagData[slot] = m_nullobj;
        addDirty(slot, reason, false);
    }
    else
    {
        if (m_bagData[slot].itemCount < slotData->itemCount)
            addItemChangeLog(reason, slotData->itemId, slotData->itemClass,
                slotData->srcId, slotData->itemCount - m_bagData[slot].itemCount, 1, 0);
        else
            addItemChangeLog(reason, slotData->itemId, slotData->itemClass,
                slotData->srcId, m_bagData[slot].itemCount - slotData->itemCount, 2, 0);
        m_bagData[slot] = *slotData;
        bool bNeedCreate = (slotData->itemClass == 2 && slotData->srcId == 0);
        addDirty(slot, reason, bNeedCreate);
    }
}

void CExtCharBag::CleanSlot(int32_t slot, ITEM_CHANGE_REASON reason)
{
    if (!IsSlotValid(slot) || m_bagData[slot].itemId <= 0) return;
    addItemChangeLog(reason, m_bagData[slot].itemId, m_bagData[slot].itemClass,
        m_bagData[slot].srcId, m_bagData[slot].itemCount, 2, 0);
    if (m_bagData[slot].srcId > 0 && m_bagData[slot].itemClass == 2)
    {
        int64_t srcId = m_bagData[slot].srcId;
        int8_t connId = Player::getConnId(m_pPlayer);
        CEquipManager* mgr = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::DeleteMemEquip(mgr, connId, srcId, reason);
    }
    m_bagData[slot] = m_nullobj;
    addDirty(slot, reason, false);
    saveItemChangeLog();
}

// ============================================================================
// 日志与事务支持
// ============================================================================

void CExtCharBag::addItemChangeLog(ITEM_CHANGE_REASON reason, int32_t itemId, int8_t itemClass,
    int64_t srcId, int32_t count, int8_t flag, int32_t allCount)
{
    LogItemChange log;
    memset(&log, 0, sizeof(log));
    log.reason = reason;
    log.id = itemId;
    log.itemClass = itemClass;
    log.srcId = srcId;
    log.count = count;
    log.flag = flag;
    log.allCount = allCount;
    if (m_pPlayer)
    {
        log.cid = Player::getCid(m_pPlayer);
        log.time = (int32_t)Unit::getNow(m_pPlayer);
    }
    m_lstItemChangeLog.push_back(log);
}

void CExtCharBag::saveItemChangeLog()
{
    if (!m_pPlayer || m_lstItemChangeLog.empty()) return;
    int8_t connId = Player::getConnId(m_pPlayer);
    DBService* db = Answer::Singleton<DBService>::instance();
    for (auto& log : m_lstItemChangeLog)
    {
        DBService::insertItemChange(db, connId, &log);
        if (log.itemClass == 2)
        {
            // 装备日志
            Log360 stu;
            // 注意: Log360 含 std::string 成员, 不能用 memset
            // 改用 placement-new 重建字符串后赋值
            new (&stu.passport) std::string();
            new (&stu.LogString) std::string();
            new (&stu.Interface) std::string();
            stu.UseSid = Player::getSid(m_pPlayer);
            Player::GetPassport(m_pPlayer, &stu.passport);
            // 构建日志字符串
            char logStr[256];
            snprintf(logStr, sizeof(logStr), "equip_change:id=%d,count=%d,flag=%d,srcId=%lld",
                log.id, log.count, log.flag, (long long)log.srcId);
            stu.LogString = logStr;
            DBService::log360(db, connId, &stu);
        }
    }
    m_lstItemChangeLog.clear();
}

void CExtCharBag::clearItemChangeLog()
{
    m_lstItemChangeLog.clear();
}

void CExtCharBag::backUpBagData()
{
    memcpy(m_bagBackUp, m_bagData, sizeof(m_bagBackUp));
    saveItemChangeLog();
}

void CExtCharBag::recoverBagData()
{
    memcpy(m_bagData, m_bagBackUp, sizeof(m_bagData));
    clearItemChangeLog();
}

// ============================================================================
// 比较函数
// ============================================================================

bool CExtCharBag::compairItem(const MemChrBag* bagSlot, const ItemData* data) const
{
    if (!bagSlot || !data) return false;
    return bagSlot->itemId == data->m_nId && bagSlot->itemClass == data->m_nClass;
}

bool CExtCharBag::compairSlot(const MemChrBag* left, const MemChrBag* right) const
{
    if (!left || !right) return false;
    return left->itemId == right->itemId
        && left->itemClass == right->itemClass
        && left->bind == right->bind;
}

// ============================================================================
// 核心添加逻辑
// ============================================================================

bool CExtCharBag::addItem(const MemChrBag* item, ITEM_CHANGE_REASON nReason)
{
    int8_t itemClass = item->itemClass;
    int32_t itemId = item->itemId;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nLayNum = CfgData::getOverlay(cfg, itemId, itemClass);
    if (nLayNum <= 0) return false;
    int32_t nCount = item->itemCount;
    if (nCount < 0) return false;
    if (nCount == 0) return true;

    int32_t nBagSize = GetBagSize();
    // 先尝试堆叠到已有物品上
    if (nLayNum > 1)
    {
        for (int32_t slot = 0; slot < nBagSize; ++slot)
        {
            MemChrBag slotData = *GetSlotData(slot);
            if (compairSlot(item, &slotData))
            {
                int32_t __a = nLayNum - slotData.itemCount;
                int32_t nAddon = (__a < nCount) ? __a : nCount;
                if (nAddon > 0)
                {
                    slotData.itemCount += nAddon;
                    setSlotData(slot, &slotData, nReason, 0);
                    nCount -= nAddon;
                    if (nCount <= 0)
                        return true;
                }
            }
        }
    }
    // 再找空位放置
    MemChrBag bagSlot;
    memset(&bagSlot, 0, sizeof(bagSlot));
    for (int32_t i = 0; i < nBagSize; ++i)
    {
        bagSlot = *GetSlotData(i);
        if (IsEmptySlot(&bagSlot))
        {
            bagSlot = *item;
            if (nCount <= nLayNum)
            {
                bagSlot.itemCount = nCount;
                setSlotData(i, &bagSlot, nReason, nCount);
                return true;
            }
            bagSlot.itemCount = nLayNum;
            nCount -= nLayNum;
            setSlotData(i, &bagSlot, nReason, nLayNum);
        }
    }
    return false;
}

bool CExtCharBag::addItem(const MemChrBagVector* vItem, ITEM_CHANGE_REASON nReason)
{
    for (size_t i = 0; i < vItem->size(); ++i)
    {
        if (!addItem(&(*vItem)[i], nReason))
            return false;
    }
    return true;
}

bool CExtCharBag::AddItem(const MemChrBag* item, ITEM_CHANGE_REASON nReason)
{
    if (autoUseItem(item, nReason))
        return true;
    backUpBagData();
    if (!addItem(item, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddItem(const MemChrBagVector* vItem, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!addItem(vItem, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

// ============================================================================
// 核心移除逻辑
// ============================================================================

bool CExtCharBag::removeItem(const ItemData* data, ITEM_CHANGE_REASON nReason,
    int32_t* BindCount, int32_t* UnBindCount)
{
    int32_t nCount = data->m_nCount;
    if (nCount <= 0) return false;
    int32_t nBagSize = GetBagSize();

    // 先移除绑定的
    for (int32_t i = 0; i < nBagSize; ++i)
    {
        MemChrBag bagSlot = *GetSlotData(i);
        bool expired = (bagSlot.endTime > 0 && m_pPlayer && bagSlot.endTime <= Unit::getNow(m_pPlayer));
        if (!expired && compairItem(&bagSlot, data) && bagSlot.bind == 1)
        {
            int32_t nRemove = (bagSlot.itemCount >= nCount) ? nCount : bagSlot.itemCount;
            bagSlot.itemCount -= nRemove;
            if (BindCount) *BindCount += nRemove;
            setSlotData(i, &bagSlot, nReason, nRemove);
            nCount -= nRemove;
            if (nCount <= 0) return true;
        }
    }
    // 再移除未绑定的
    for (int32_t j = 0; j < nBagSize; ++j)
    {
        MemChrBag bagSlot = *GetSlotData(j);
        bool expired = (bagSlot.endTime > 0 && m_pPlayer && bagSlot.endTime <= Unit::getNow(m_pPlayer));
        if (!expired && compairItem(&bagSlot, data) && !bagSlot.bind)
        {
            int32_t nRemove = (bagSlot.itemCount >= nCount) ? nCount : bagSlot.itemCount;
            bagSlot.itemCount -= nRemove;
            if (UnBindCount) *UnBindCount += nRemove;
            setSlotData(j, &bagSlot, nReason, nRemove);
            nCount -= nRemove;
            if (nCount <= 0) return true;
        }
    }
    return false;
}

bool CExtCharBag::removeItem(const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    int32_t BindCount = 0, UnBindCount = 0;
    return removeItem(data, nReason, &BindCount, &UnBindCount);
}

bool CExtCharBag::removeItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    for (auto& item : *lst)
    {
        if (!removeItem(&item, nReason))
            return false;
    }
    return true;
}

bool CExtCharBag::removeItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason,
    int32_t* BindCount, int32_t* UnBindCount)
{
    int32_t nCount = data->m_nCount;
    if (nCount <= 0) return false;
    for (size_t i = 0; i < vSlot->size(); ++i)
    {
        int32_t idx = (*vSlot)[i];
        if (!IsSlotValid(idx)) continue;
        MemChrBag bagSlot = *GetSlotData(idx);
        bool expired = (bagSlot.endTime > 0 && m_pPlayer && bagSlot.endTime <= Unit::getNow(m_pPlayer));
        if (!expired && compairItem(&bagSlot, data))
        {
            int32_t nRemove = (bagSlot.itemCount >= nCount) ? nCount : bagSlot.itemCount;
            bagSlot.itemCount -= nRemove;
            if (bagSlot.bind == 1 || bagSlot.bind == 2)
            { if (BindCount) *BindCount += nRemove; }
            else
            { if (UnBindCount) *UnBindCount += nRemove; }
            setSlotData(idx, &bagSlot, nReason, nRemove);
            nCount -= nRemove;
            if (nCount <= 0) return true;
        }
    }
    return false;
}

bool CExtCharBag::removeItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    int32_t BindCount = 0, UnBindCount = 0;
    return removeItem(vSlot, data, nReason, &BindCount, &UnBindCount);
}

bool CExtCharBag::removeItem(const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    for (auto& item : *lst)
    {
        if (!removeItem(vSlot, &item, nReason))
            return false;
    }
    return true;
}

// RemoveItem 公开包装器 (事务性: 备份->操作->恢复/保存)
bool CExtCharBag::RemoveItem(const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(data, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::RemoveItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(lst, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::RemoveItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    int32_t b = 0, u = 0;
    return RemoveItem(vSlot, data, nReason, &b, &u);
}

bool CExtCharBag::RemoveItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason,
    int32_t* BindCount, int32_t* UnBindCount)
{
    backUpBagData();
    if (!removeItem(vSlot, data, nReason, BindCount, UnBindCount))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::RemoveItem(const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(vSlot, lst, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::RemoveItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason,
    int32_t* BindCount, int32_t* UnBindCount)
{
    backUpBagData();
    for (auto& item : *lst)
    {
        if (!removeItem(&item, nReason, BindCount, UnBindCount))
        {
            recoverBagData();
            return false;
        }
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::RemoveItem(const ItemData* data, ITEM_CHANGE_REASON nReason,
    int32_t* BindCount, int32_t* UnBindCount)
{
    backUpBagData();
    if (!removeItem(data, nReason, BindCount, UnBindCount))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::RemoveItem(const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason,
    int32_t* BindCount, int32_t* UnBindCount)
{
    backUpBagData();
    for (auto& item : *lst)
    {
        if (!removeItem(vSlot, &item, nReason, BindCount, UnBindCount))
        {
            recoverBagData();
            return false;
        }
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::RemoveCombiItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeCombiItem(lst, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::removeCombiItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    int32_t nBagSize = GetBagSize();
    for (auto& itemData : *lst)
    {
        int32_t nCount = itemData.m_nCount;
        // 先移除绑定
        for (int32_t i = 0; i < nBagSize; ++i)
        {
            const MemChrBag* bagSlot = GetSlotData(i);
            if (compairItem(bagSlot, &itemData) && (bagSlot->bind == 1 || bagSlot->bind == 2))
            {
                MemChrBag tSlot = *bagSlot;
                int32_t nRemove = (tSlot.itemCount >= nCount) ? nCount : tSlot.itemCount;
                tSlot.itemCount -= nRemove;
                setSlotData(i, &tSlot, nReason, nRemove);
                nCount -= nRemove;
                if (nCount <= 0) break;
            }
        }
        if (nCount <= 0) continue;
        // 再移除未绑定
        for (int32_t j = 0; j < nBagSize; ++j)
        {
            const MemChrBag* bagSlot = GetSlotData(j);
            if (compairItem(bagSlot, &itemData) && bagSlot->bind != 1 && bagSlot->bind != 2)
            {
                MemChrBag tSlot = *bagSlot;
                int32_t nRemove = (tSlot.itemCount >= nCount) ? nCount : tSlot.itemCount;
                tSlot.itemCount -= nRemove;
                setSlotData(j, &tSlot, nReason, nRemove);
                nCount -= nRemove;
                if (nCount <= 0) break;
            }
        }
        if (nCount > 0) return false;
    }
    return true;
}

// ============================================================================
// 批量增减 (AddAndRemoveItem)
// ============================================================================

bool CExtCharBag::AddAndRemoveItem(const MemChrBag* item, const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(data, nReason) || !addItem(item, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddAndRemoveItem(const MemChrBag* item, const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(lst, nReason) || !addItem(item, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddAndRemoveItem(const MemChrBag* item, const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(vSlot, data, nReason) || !addItem(item, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddAndRemoveItem(const MemChrBag* item, const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(vSlot, lst, nReason) || !addItem(item, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddAndRemoveItem(const MemChrBagVector* vItem, const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(data, nReason) || !addItem(vItem, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddAndRemoveItem(const MemChrBagVector* vItem, const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(lst, nReason) || !addItem(vItem, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddAndRemoveItem(const MemChrBagVector* vItem, const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(vSlot, data, nReason) || !addItem(vItem, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

bool CExtCharBag::AddAndRemoveItem(const MemChrBagVector* vItem, const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason)
{
    backUpBagData();
    if (!removeItem(vSlot, lst, nReason) || !addItem(vItem, nReason))
    {
        recoverBagData();
        return false;
    }
    saveItemChangeLog();
    return true;
}

// ============================================================================
// 脏标记系统
// ============================================================================

void CExtCharBag::addDirty(int32_t nSlot, int32_t reason, bool bNeedCreate)
{
    if (bNeedCreate)
        m_lstWaitCreate.push_back({nSlot, reason});
    for (auto& entry : m_lstDirty)
    {
        if (entry.nSlot == nSlot)
            return;
    }
    m_lstDirty.push_back({nSlot, (int8_t)(reason > 0)});
}

void CExtCharBag::clearDirty()
{
    m_lstDirty.clear();
}

void CExtCharBag::ForceSendDirty()
{
    if (sendDirty()) clearDirty();
}

void CExtCharBag::checkDirty()
{
    if (sendDirty()) clearDirty();
}

bool CExtCharBag::sendDirty()
{
    if (!m_pPlayer || m_lstDirty.empty()) return false;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2DB7);
    if (!packet) return false;
    Answer::NetPacket::writeInt8(packet, 1);
    uint32_t nOffSet = Answer::NetPacket::getWOffset(packet);
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);
    for (auto& dirty : m_lstDirty)
    {
        Answer::NetPacket::writeInt8(packet, dirty.bChange);
        int32_t nSlot = dirty.nSlot;
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
    int8_t cid = Player::getConnId(m_pPlayer);
    GameService::sendPacketTo(gs, cid, gateIndex, packet);
    return true;
}

void CExtCharBag::checkCreate()
{
    if (!m_pPlayer || m_lstWaitCreate.empty()) return;
    std::vector<MemEquip> vEquip;
    vEquip.reserve(m_lstWaitCreate.size());
    for (auto it = m_lstWaitCreate.begin(); it != m_lstWaitCreate.end(); )
    {
        bool created = false;
        if (it->nSlot >= 0 && it->nSlot < BAG_SIZE)
        {
            MemChrBag* bagSlot = &m_bagData[it->nSlot];
            if (bagSlot->srcId <= 0 && bagSlot->itemClass == 2)
            {
                int8_t connId = Player::getConnId(m_pPlayer);
                CEquipManager* mgr = Answer::Singleton<CEquipManager>::instance();
                MemEquip equip;
                memset(&equip, 0, sizeof(equip));
                if (CEquipManager::CreateMemEquip(mgr, &equip, connId, it->nReason,
                    bagSlot->itemId, bagSlot->srcId, bagSlot->bind, bagSlot->endTime))
                {
                    if (equip.id > 0)
                    {
                        MemChrBag tSlot = *bagSlot;
                        tSlot.srcId = equip.id;
                        setSlotData(it->nSlot, &tSlot, (ITEM_CHANGE_REASON)it->nReason, 0);
                        vEquip.push_back(equip);
                        created = true;
                    }
                }
            }
        }
        if (created)
            it = m_lstWaitCreate.erase(it);
        else
            ++it;
    }
    if (!vEquip.empty())
    {
        CEquipManager* mgr = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::SendPlayerEquipInfo(mgr, m_pPlayer, &vEquip);
    }
}

// ============================================================================
// 广播
// ============================================================================

void CExtCharBag::sendUseBroadcast(int32_t broadcastId, const std::string* pName, int64_t cid, int32_t itemId)
{
    if (!m_pPlayer) return;
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, (uint16_t)broadcastId);
    if (!packet) return;
    Answer::NetPacket::writeUTF8(packet, pName);
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, itemId);
    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);
    GameService::worldBroadcast(gs, 0, packet);
}

// ============================================================================
// canUseItem
// ============================================================================

bool CExtCharBag::canUseItem(const void* pCfgItem)
{
    if (!m_pPlayer || !pCfgItem) return false;
    const auto* cfgItem = (const CfgItem*)pCfgItem;
    if (cfgItem->level > 0)
    {
        if (Player::getLevel(m_pPlayer) < cfgItem->level)
            return false;
    }
    if (cfgItem->job > 0)
    {
        if (Player::getJob(m_pPlayer) != cfgItem->job)
            return false;
    }
    if (cfgItem->cd_group > 0)
    {
        int64_t nowTick = Unit::getTick(m_pPlayer);
        int64_t lastTick = m_lastItemTick[cfgItem->cd_group];
        if (lastTick > 0 && nowTick - lastTick < 1000)
            return false;
    }
    return true;
}

// ============================================================================
// 自动使用 (货币/礼包)
// ============================================================================

bool CExtCharBag::isAutoUseItem(const MemChrBag* item) const
{
    if (!item) return false;
    if (item->itemClass == 4) return true;
    if (item->itemClass != 1) return false;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgItem* pCfgItem = CfgData::getItem(cfg, item->itemId);
    if (!pCfgItem) return false;
    int32_t type = pCfgItem->type;
    return type == 1 || type == 86;
}

bool CExtCharBag::autoUseItem(const MemChrBag* item, ITEM_CHANGE_REASON nReason)
{
    if (!m_pPlayer || !item) return false;
    int32_t nCount = item->itemCount;
    if (nCount <= 0) return false;

    if (item->itemClass == 4)
    {
        if (item->itemId == 6)
        {
            CExtOperateLimit* limit = Player::GetOperateLimit(m_pPlayer);
            CExtOperateLimit::AddLimitCount(limit, 37209, nCount);
            return true;
        }
        CURRENCY_TYPE ctype = CItemHelper::TranseCurrencyType((CURRENCY_ITEM_ID)item->itemId);
        if (ctype == CURRENCY_INVALID) return false;
        CURRENCY_CHANGE_REASON nCCReason = CURRENCY_CHANGE_REASON::MCR_AUTO_USE;
        switch (nReason)
        {
            case ICR_PICK: nCCReason = CURRENCY_CHANGE_REASON::MCR_PICK_CURRENCY_ITEM; break;
            case ICR_MAIL:
            case ICR_AUCTION_BACK: nCCReason = CURRENCY_CHANGE_REASON::MCR_MAIL_CURRENCY_ITEM; break;
            case ICR_CROSS_DRAW_REWARD: nCCReason = CURRENCY_CHANGE_REASON::MCR_CROSS_DRAW_REWARD; break;
            default: break;
        }
        return Player::AddCurrency(m_pPlayer, ctype, nCount, nCCReason, 0);
    }
    if (item->itemClass == 1)
    {
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        const CfgItem* pCfgItem = CfgData::getItem(cfg, item->itemId);
        if (pCfgItem)
        {
            int32_t type = pCfgItem->type;
            if (type == 1 || type == 86)
            {
                int32_t itemCount = item->itemCount;
                ItemEffectManager* mgr = Answer::Singleton<ItemEffectManager>::instance();
                ItemEffectManager::effect(mgr, item->itemId, m_pPlayer, m_pPlayer, &itemCount);
                return true;
            }
        }
    }
    return false;
}

// ============================================================================
// AddItemsAndMingGe
// ============================================================================

bool CExtCharBag::AddItemsAndMingGe(const MemChrBagVector* vItem, ITEM_CHANGE_REASON nReason)
{
    int32_t freeSlots = GetFreeSlotCount();
    if (freeSlots < (int32_t)vItem->size()) return false;
    for (size_t i = 0; i < vItem->size(); ++i)
    {
        if (!AddItem(&(*vItem)[i], nReason))
            return false;
    }
    return true;
}

// ============================================================================
// 商业操作
// ============================================================================

int32_t CExtCharBag::buyChrShopItem(void* pCharShop, int32_t id, int32_t count)
{
    if (!m_pPlayer || !pCharShop) return 10002;
    CfgChrShop* pShop = (CfgChrShop*)pCharShop;
    if (pShop->Price <= 0 || count <= 0) return 10002;
    if (pShop->LimitCount > 0 && count + getLimitCount(pShop->Index) > pShop->LimitCount)
        return 10002;
    if (GetFreeSlotCount() <= 0) return 10002;

    MemChrBag bagItem;
    memset(&bagItem, 0, sizeof(bagItem));
    bagItem.itemId = pShop->ItemId;
    bagItem.itemClass = pShop->ItemClass;
    bagItem.itemCount = count;
    bagItem.bind = pShop->IsBind;

    int32_t costValue = count * pShop->Price;
    if (pShop->ConstType > 12 || pShop->ConstType < -1) return 10002;

    // 检查心魔等级
    CXinMo* xm = Player::GetCXinMo(m_pPlayer);
    if (CXinMo::GetXinMoLevel(xm) < pShop->QiQinglevel) return 10002;

    // 扣除货币
    if (pShop->ConstType && pShop->ConstType != 6)
    {
        if (!Player::DecCurrency(m_pPlayer, (CURRENCY_TYPE)pShop->ConstType, costValue, CURRENCY_CHANGE_REASON::MCR_CHR_SHOP_COST, pShop->ItemId))
            return 10002;
    }
    else
    {
        if (!Player::DecMoneyAndNoBind(m_pPlayer, costValue, CURRENCY_CHANGE_REASON::MCR_CHR_SHOP_COST, pShop->ItemId))
            return 10002;
    }

    // 添加物品
    if (!AddItem(&bagItem, ICR_SHOP_BUY))
    {
        int64_t now = Unit::getNow(m_pPlayer);
        int64_t cid = Player::getCid(m_pPlayer);
        Answer::Logger::print(Answer::LOG_LEVEL_ERROR, "buyChrShopItem fail",
            bagItem.itemClass, bagItem.itemId, count, now, cid);
    }

    if (pShop->LimitCount > 0)
        addLimitCount(pShop->Index, count);

    Player::sendBuyItemInfo(m_pPlayer, bagItem.itemId, bagItem.itemClass, count, costValue, pShop->Index);

    // 日志
    Log360 stu;
    new (&stu.passport) std::string();
    new (&stu.LogString) std::string();
    new (&stu.Interface) std::string();
    stu.UseSid = Player::getSid(m_pPlayer);
    Player::GetPassport(m_pPlayer, &stu.passport);
    char logStr[1024];
    int64_t money = Player::GetMoneyBindAndNoBind(m_pPlayer);
    int32_t mapId = StaticObj::getMapId(m_pPlayer);
    int32_t level = Player::getLevel(m_pPlayer);
    snprintf(logStr, sizeof(logStr),
        "balance=%lld&itemid=%d:%d&amount=%d&money=%d&level=%d&remain=&map_id=%d",
        (long long)money, bagItem.itemClass, bagItem.itemId, count, costValue, level, mapId);
    stu.LogString = logStr;
    int8_t connId = Player::getConnId(m_pPlayer);
    DBService* db = Answer::Singleton<DBService>::instance();
    DBService::log360(db, connId, &stu);
    return 0;
}

int32_t CExtCharBag::buyBackChrShopItem(void* pCharShop, int32_t index)
{
    if (!m_pPlayer || !pCharShop) return 10002;
    if (index < 0 || index >= SELL_SIZE) return 10002;
    const MemChrBag* sellItem = &m_sellItemInfo[index];
    if (sellItem->itemCount <= 0) return 10002;
    if (!AddItem(sellItem, ICR_BUY_BACK))
        return 10002;
    deleteSellItem(index);
    SendBagSellItem();
    return 0;
}

void CExtCharBag::CleanItemId(int32_t nId, int32_t nReason, int8_t nClass)
{
    int32_t bagSize = GetBagSize();
    for (int32_t i = 0; i < bagSize; ++i)
    {
        if (m_bagData[i].itemId == nId && m_bagData[i].itemClass == nClass)
            CleanSlot(i, (ITEM_CHANGE_REASON)nReason);
    }
    saveItemChangeLog();
}

// ============================================================================
// 网络协议处理器
// ============================================================================

int32_t CExtCharBag::onUseItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    int32_t slot = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readInt32(inPacket);  // type
    int32_t nItemId = Answer::NetPacket::readInt32(inPacket);
    int32_t nCount = 1;
    MemChrBag slotData = *GetSlotData(slot);
    if (slotData.itemCount <= 0 || slotData.itemClass != 1 || slotData.itemId != nItemId)
        return 10002;
    if (slotData.endTime > 0 && Unit::getNow(m_pPlayer) > slotData.endTime)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgItem* cfgItem = CfgData::getItem(cfg, slotData.itemId);
    if (!cfgItem) return 10002;
    if (!canUseItem(cfgItem)) return 10002;
    int32_t err = 10002;
    ItemEffectManager* mgr = Answer::Singleton<ItemEffectManager>::instance();
    err = ItemEffectManager::effect(mgr, slotData.itemId, m_pPlayer, m_pPlayer, &nCount);
    if (!err)
    {
        ChrTask* task = Player::GetTask(m_pPlayer);
        ChrTask::updateTaskUseItem(task, slotData.itemId, nCount);
        slotData.itemCount -= 1;
        SetSlotData(slot, &slotData, ICR_BAG_USE, nCount);
        m_lastItemTick[cfgItem->cd_group] = Unit::getTick(m_pPlayer);
        if (cfgItem->useBroadcast > 0)
        {
            int32_t itemId = cfgItem->id;
            int64_t cid = Player::getCid(m_pPlayer);
            std::string p_name;
            Player::getName(m_pPlayer, &p_name);
            sendUseBroadcast(cfgItem->useBroadcast, &p_name, cid, itemId);
        }
        int16_t gateIndex = Player::getGateIndex(m_pPlayer);
        int8_t connId = Player::getConnId(m_pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        GameService::replySuccess(gs, connId, gateIndex, 0x59, cfgItem->id);
    }
    return err;
}

int32_t CExtCharBag::onPatchUseItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    int32_t slot = Answer::NetPacket::readInt32(inPacket);
    int32_t count = Answer::NetPacket::readInt32(inPacket);
    int32_t nItemId = Answer::NetPacket::readInt32(inPacket);
    if (count <= 0) return 10002;
    MemChrBag slotData = *GetSlotData(slot);
    if (slotData.itemCount <= 0 || slotData.itemClass != 1 || slotData.itemId != nItemId)
        return 10002;
    if (slotData.itemCount < count)
        count = slotData.itemCount;
    if (slotData.endTime > 0 && Unit::getNow(m_pPlayer) > slotData.endTime)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgItem* cfgItem = CfgData::getItem(cfg, slotData.itemId);
    if (!cfgItem || !canUseItem(cfgItem)) return 10002;
    int32_t err = 0;
    if (cfgItem->type == 106 || cfgItem->type == 108)
    {
        for (int32_t i = 0; i < count; ++i)
        {
            int32_t tmpCount = 1;
            ItemEffectManager* mgr = Answer::Singleton<ItemEffectManager>::instance();
            err = ItemEffectManager::effect(mgr, slotData.itemId, m_pPlayer, m_pPlayer, &tmpCount);
            if (err) break;
            ChrTask* task = Player::GetTask(m_pPlayer);
            ChrTask::updateTaskUseItem(task, slotData.itemId, tmpCount);
            slotData.itemCount -= tmpCount;
            SetSlotData(slot, &slotData, ICR_BAG_USE, tmpCount);
        }
    }
    else
    {
        ItemEffectManager* mgr = Answer::Singleton<ItemEffectManager>::instance();
        err = ItemEffectManager::effect(mgr, slotData.itemId, m_pPlayer, m_pPlayer, &count);
        if (!err)
        {
            ChrTask* task = Player::GetTask(m_pPlayer);
            ChrTask::updateTaskUseItem(task, slotData.itemId, count);
            slotData.itemCount -= count;
            SetSlotData(slot, &slotData, ICR_BAG_USE, count);
        }
    }
    m_lastItemTick[cfgItem->cd_group] = Unit::getTick(m_pPlayer);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, 0x5A, cfgItem->id);
    return err;
}

int32_t CExtCharBag::onSelectItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    int32_t nSlot = Answer::NetPacket::readInt32(inPacket);
    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    MemChrBag slotData = *GetSlotData(nSlot);
    if (slotData.itemCount <= 0) return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgItem* pItem = CfgData::getItem(cfg, slotData.itemId);
    if (!pItem || pItem->type != 213) return 10002;
    int32_t Effect = atoi(pItem->effect.c_str());
    SelectItemCfg* pItems = CfgData::GetSelectItemCfg(cfg, Effect);
    if (!pItems) return 10002;
    Int32Vector vSlot;
    vSlot.push_back(nSlot);
    ItemData removeData;
    removeData.m_nId = slotData.itemId;
    removeData.m_nClass = slotData.itemClass;
    removeData.m_nCount = 1;
    MemChrBag GetItemStu = pItems->vItem[nId];
    if (GetFreeSlotCount() <= 0) return 10002;
    if (!RemoveItem(&vSlot, &removeData, ICR_SELECT_ITEM))
        return 10002;
    MemChrBagVector stu;
    stu.push_back(GetItemStu);
    if (!AddItemsAndMingGe(&stu, ICR_SELECT_ITEM))
        return 10002;
    uint16_t proc = Answer::NetPacket::getProc(inPacket);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, proc, nId);
    return 0;
}

int32_t CExtCharBag::onDiscardItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    int32_t slot = Answer::NetPacket::readInt32(inPacket);
    int32_t count = Answer::NetPacket::readInt32(inPacket);
    if (!IsSlotValid(slot)) return 10002;
    MemChrBag slotData = *GetSlotData(slot);
    if (count <= 0 || slotData.itemCount < count) return 10002;
    if (slotData.bind) return 10002;
    slotData.itemCount -= count;
    if (slotData.itemCount <= 0)
        CleanSlot(slot, ICR_ITEM_DISCARD);
    else
        SetSlotData(slot, &slotData, ICR_ITEM_DISCARD, count);
    uint16_t proc = Answer::NetPacket::getProc(inPacket);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, proc, 0);
    return 0;
}

int32_t CExtCharBag::onUseMutiItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    int32_t slot = Answer::NetPacket::readInt32(inPacket);
    int32_t count = Answer::NetPacket::readInt32(inPacket);
    int32_t nItemId = Answer::NetPacket::readInt32(inPacket);
    if (count <= 0) return 10002;
    MemChrBag slotData = *GetSlotData(slot);
    if (slotData.itemCount <= 0 || slotData.itemClass != 1 || slotData.itemId != nItemId)
        return 10002;
    if (slotData.itemCount < count) count = slotData.itemCount;
    if (slotData.endTime > 0 && Unit::getNow(m_pPlayer) > slotData.endTime)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgItem* cfgItem = CfgData::getItem(cfg, slotData.itemId);
    if (!cfgItem || !canUseItem(cfgItem)) return 10002;
    int32_t err = 0;
    for (int32_t i = 0; i < count; ++i)
    {
        int32_t tmpCount = 1;
        ItemEffectManager* mgr = Answer::Singleton<ItemEffectManager>::instance();
        err = ItemEffectManager::effect(mgr, slotData.itemId, m_pPlayer, m_pPlayer, &tmpCount);
        if (err) break;
        ChrTask* task = Player::GetTask(m_pPlayer);
        ChrTask::updateTaskUseItem(task, slotData.itemId, tmpCount);
        slotData.itemCount -= tmpCount;
        SetSlotData(slot, &slotData, ICR_USE_MULTI_ITEM, tmpCount);
    }
    m_lastItemTick[cfgItem->cd_group] = Unit::getTick(m_pPlayer);
    uint16_t proc = Answer::NetPacket::getProc(inPacket);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, proc, 0);
    return err;
}

int32_t CExtCharBag::onMoveItem(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    int32_t oldslot = Answer::NetPacket::readInt32(inPacket);
    int32_t newslot = Answer::NetPacket::readInt32(inPacket);
    if (!IsSlotValid(oldslot) || !IsSlotValid(newslot) || oldslot == newslot)
        return 10002;
    MemChrBag oldSlotData = *GetSlotData(oldslot);
    if (oldSlotData.itemCount <= 0) return 10002;
    MemChrBag newSlotData = *GetSlotData(newslot);
    if (newSlotData.itemCount <= 0)
    {
        setSlotData(newslot, &oldSlotData, ICR_NONE, 0);
        setSlotData(oldslot, &m_nullobj, ICR_NONE, 0);
    }
    else if (compairSlot(&oldSlotData, &newSlotData))
    {
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        int32_t overlay = CfgData::getOverlay(cfg, oldSlotData.itemId, oldSlotData.itemClass);
        if (overlay <= 0 || newSlotData.itemCount >= overlay)
            return 10002;
        int32_t addon = (oldSlotData.itemCount < overlay - newSlotData.itemCount)
            ? oldSlotData.itemCount : (overlay - newSlotData.itemCount);
        if (addon < 0) return 10002;
        newSlotData.itemCount += addon;
        setSlotData(newslot, &newSlotData, ICR_NONE, 0);
        oldSlotData.itemCount -= addon;
        setSlotData(oldslot, &oldSlotData, ICR_NONE, 0);
    }
    else
    {
        MemChrBag tmp = newSlotData;
        setSlotData(newslot, &oldSlotData, ICR_NONE, 0);
        setSlotData(oldslot, &tmp, ICR_NONE, 0);
    }
    return 0;
}

int32_t CExtCharBag::onSplitItem(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    int32_t slot = Answer::NetPacket::readInt32(inPacket);
    int32_t count = Answer::NetPacket::readInt32(inPacket);
    if (!IsSlotValid(slot)) return 10002;
    MemChrBag slotData = *GetSlotData(slot);
    if (count <= 0 || slotData.itemCount < count || slotData.itemCount <= 0)
        return 10002;
    int32_t freeSlot = GetFirstFreeSlot();
    if (!IsSlotValid(freeSlot)) return 10002;
    MemChrBag newSlotData;
    memset(&newSlotData, 0, sizeof(newSlotData));
    newSlotData.itemId = slotData.itemId;
    newSlotData.itemClass = slotData.itemClass;
    newSlotData.bind = slotData.bind;
    newSlotData.endTime = slotData.endTime;
    newSlotData.srcId = slotData.srcId;
    newSlotData.itemCount = count;
    setSlotData(freeSlot, &newSlotData, ICR_NONE, 0);
    slotData.itemCount -= count;
    setSlotData(slot, &slotData, ICR_NONE, 0);
    return 0;
}

int32_t CExtCharBag::onSort(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    std::vector<MemChrBag> items;
    int32_t nBagSize = GetBagSize();
    // 收集所有物品并合并可堆叠的
    for (int32_t i = 0; i < nBagSize; ++i)
    {
        const MemChrBag* slotdata = GetSlotData(i);
        if (slotdata->itemCount > 0)
        {
            bool bFind = false;
            for (auto& existing : items)
            {
                if (compairSlot(&existing, slotdata))
                {
                    existing.itemCount += slotdata->itemCount;
                    bFind = true;
                    break;
                }
            }
            if (!bFind)
                items.push_back(*slotdata);
        }
    }
    // 排序
    std::sort(items.begin(), items.end(), GreaterItem);
    // 回写
    int32_t slot = 0;
    for (auto& bagSlot : items)
    {
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        int32_t overlay = CfgData::getOverlay(cfg, bagSlot.itemId, bagSlot.itemClass);
        if (overlay > 0)
        {
            while (bagSlot.itemCount > overlay && slot < nBagSize)
            {
                MemChrBag tSlot = bagSlot;
                tSlot.itemCount = overlay;
                bagSlot.itemCount -= overlay;
                setSlotData(slot++, &tSlot, ICR_NONE, 0);
            }
            if (bagSlot.itemCount > 0 && slot < nBagSize)
                setSlotData(slot++, &bagSlot, ICR_NONE, 0);
        }
    }
    while (slot < nBagSize)
        setSlotData(slot++, &m_nullobj, ICR_NONE, 0);
    return 0;
}

int32_t CExtCharBag::onSellItem(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 2;
    int64_t nTotalValue = 0;
    int32_t nSize = Answer::NetPacket::readInt32(inPacket);
    for (int32_t i = 0; i < nSize; ++i)
    {
        int32_t nSlot = Answer::NetPacket::readInt32(inPacket);
        const MemChrBag* slotData = GetSlotData(nSlot);
        if (slotData->itemCount <= 0) return 10002;
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        if (!CfgData::canSell(cfg, slotData->itemId, slotData->itemClass))
            return 10002;
        int64_t outValue = CfgData::getOutValue(cfg, slotData->itemId, slotData->itemClass);
        if (outValue < 0) return 10002;
        outValue *= slotData->itemCount;
        nTotalValue += outValue;
        setSellItem(slotData);
        setSlotData(nSlot, &m_nullobj, ICR_SELL, 0);
        Player::AddCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_MONEY, outValue, CURRENCY_CHANGE_REASON::MCR_NPC_SELL, slotData->itemId);
    }
    SendBagSellItem();
    uint16_t proc = Answer::NetPacket::getProc(inPacket);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, proc, nTotalValue);
    return 0;
}

int32_t CExtCharBag::onAddItem(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 2;
    ITEM_CHANGE_REASON nReason = (ITEM_CHANGE_REASON)Answer::NetPacket::readInt32(inPacket);
    std::string mailParam;
    Answer::NetPacket::readUTF8(inPacket, &mailParam);
    MemChrBagVector vItem;
    int32_t nSize = Answer::NetPacket::readInt32(inPacket);
    if (nSize > 0)
    {
        vItem.resize(nSize);
        for (int32_t i = 0; i < nSize; ++i)
        {
            vItem[i].itemId = Answer::NetPacket::readInt32(inPacket);
            vItem[i].itemClass = Answer::NetPacket::readInt8(inPacket);
            vItem[i].itemCount = Answer::NetPacket::readInt32(inPacket);
            vItem[i].bind = Answer::NetPacket::readInt8(inPacket);
            vItem[i].endTime = Answer::NetPacket::readInt32(inPacket);
            vItem[i].srcId = Answer::NetPacket::readInt64(inPacket);
        }
    }
    if (!AddItem(&vItem, nReason))
    {
        int32_t mailId = 6208;
        if (nReason == ICR_AUCTION_BUY)
            mailId = 6208;
        else if (nReason == ICR_AUCTION_CANCEL)
            mailId = 6209;
        int64_t cid = Player::getCid(m_pPlayer);
        int8_t connId = Player::getConnId(m_pPlayer);
        DBService* db = Answer::Singleton<DBService>::instance();
        DBService::OnSendSysMail(db, connId, cid, mailId, &vItem, nReason, &mailParam, 0);
    }
    return 0;
}

// ============================================================================
// GetDropItem (简化实现)
// ============================================================================

void CExtCharBag::GetDropItem(Player* pKiller, void* DropRate, int32_t nUsualRate,
    MemChrBagVector* ItemVector, int32_t Mid)
{
    if (!DropRate || !m_pPlayer) return;
    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 9) return;
    Int32Vector usualItemSlotVt;
    int32_t bagSize = GetBagSize();
    for (int32_t i = 0; i < bagSize; ++i)
    {
        if (m_bagData[i].itemId > 0 && m_bagData[i].itemCount > 0 && !m_bagData[i].bind)
            usualItemSlotVt.push_back(i);
    }
    if (usualItemSlotVt.empty()) return;
    std::random_shuffle(usualItemSlotVt.begin(), usualItemSlotVt.end());
    // 简化: 只处理第一个物品
    int32_t dropSlot = usualItemSlotVt[0];
    MemChrBag bagDataTmp = m_bagData[dropSlot];
    int32_t dropCount = bagDataTmp.itemCount / 2;
    if (dropCount <= 0) dropCount = 1;
    bagDataTmp.itemCount -= dropCount;
    if (bagDataTmp.itemCount > 0)
        setSlotData(dropSlot, &bagDataTmp, ICR_DIE_DROP, 0);
    else
        setSlotData(dropSlot, &m_nullobj, ICR_DIE_DROP, 0);
    bagDataTmp.itemCount = dropCount;
    ItemVector->push_back(bagDataTmp);
}
