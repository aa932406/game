#include "Other/ChouJiang.h"
#include "Other/ChouJiangTable.h"
#include "Other/ChouJiangData.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtOperateLimit.h"
#include "Other/CFunctionOpen.h"
#include "Other/COpenBeta.h"
#include "Other/CUniteServer.h"
#include "Other/CFestivalDoubleEleven.h"
#include "Other/CKiaFuRecharge.h"
#include "Other/CNationalDayHd.h"

#include <cstring>
#include <algorithm>

ChouJiang::ChouJiang()
    : CExtSystemBase()
{
    OnCleanUp();
}

ChouJiang::~ChouJiang()
{
}

void ChouJiang::OnCleanUp()
{
    m_RecordList.clear();
    m_LuckyPoint = 0;
    memset(m_ItemList, 0, sizeof(m_ItemList));
    m_WeekReward = 0;
}

void ChouJiang::OnUpdate(int64_t curTick)
{
}

void ChouJiang::OnLoadFromDB(const PlayerDBData* const dbData)
{
    m_LuckyPoint = dbData->m_ChouJinagData.m_LuckyPoint;
    m_RecordList = dbData->m_ChouJinagData.m_RecordList;
    memcpy(m_ItemList, dbData->m_ChouJinagData.m_ItemList, sizeof(m_ItemList));
    m_WeekReward = dbData->m_ChouJinagData.m_Score;
    if (m_LuckyPoint < -200)
        m_LuckyPoint = -200;
    if (m_LuckyPoint > 300)
        m_LuckyPoint = 300;
}

void ChouJiang::OnSaveToDB(PlayerDBData* const dbData)
{
    dbData->m_ChouJinagData.m_LuckyPoint = m_LuckyPoint;
    dbData->m_ChouJinagData.m_RecordList = m_RecordList;
    memcpy(dbData->m_ChouJinagData.m_ItemList, m_ItemList, sizeof(dbData->m_ChouJinagData.m_ItemList));
    dbData->m_ChouJinagData.m_Score = m_WeekReward;
}

void ChouJiang::GetInterestsProtocol(ProcIdList* const procList)
{
    procList->push_back(452);
    procList->push_back(448);
    procList->push_back(449);
    procList->push_back(450);
    procList->push_back(20083);
    procList->push_back(461);
}

void ChouJiang::OnDaySwitch(int32_t nDiffDays)
{
    Player::updateRecord(m_pPlayer, 1060, 0);
    m_WeekReward = 0;
    SendWeekRewardInfo();
}

int32_t ChouJiang::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (Answer::Singleton<GameService>::instance()->getLine() == 9)
        return 10002;
    if (nProcId == 450)
        return OnAskChouJiangRecord(inPacket);
    if (nProcId > 0x1C2)
    {
        switch (nProcId)
        {
        case 0x1CD:
            return OnGetWeekReward(inPacket);
        case 0x4E73:
            return OnChouJiang(inPacket);
        case 0x1C4:
            return OnAskChouJiang(inPacket);
        }
    }
    else
    {
        if (nProcId == 448)
            return OnTidy(inPacket);
        if (nProcId == 449)
            return OnGetItem(inPacket);
    }
    return 2;
}

int32_t ChouJiang::OnAskChouJiang(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ChouJiangTimes = inPacket->readInt8();
    int8_t AutoBuy = inPacket->readInt8();
    int8_t ChouJiangType = inPacket->readInt8();
    if (GetChouJiangType() != ChouJiangType)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const ChouJiangTable* CouJiangTable = CfgData::GetCouJiangTable(cfg);
    const ChouJiangCost* pCost = ChouJiangTable::GetChouJiangCost(CouJiangTable, ChouJiangTimes);
    if (!pCost || pCost->Items.m_nCount <= 0 || pCost->Items.m_nClass <= 0 || pCost->Items.m_nId <= 0 || pCost->ItemPrice <= 0)
        return 2;
    if (GetFreeCount() < ChouJiangTimes)
        return 2;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    int32_t HaveBindCount = Bag->HaveItemCount(&pCost->Items, 1, 1);
    int32_t HaveUnBindCount = Player::GetBag(m_pPlayer)->HaveItemCount(&pCost->Items, 0, 1);
    if (HaveUnBindCount + HaveBindCount < ChouJiangTimes)
    {
        if (AutoBuy != 1)
            return 2;
        if (Player::GetBag(m_pPlayer)->GetFreeSlotCount() <= 0)
            return 2;
        const CfgGameShop* pCfgGameShop = CfgData::GetGameShopItem(cfg, pCost->Items.m_nClass, pCost->Items.m_nId);
        if (!pCfgGameShop || pCost->ItemPrice <= 0)
            return 2;
        int32_t needBuy = ChouJiangTimes - HaveBindCount - HaveUnBindCount;
        if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < needBuy * pCfgGameShop->Price)
            return 2;
    }
    int32_t v14 = ChouJiangTimes - HaveBindCount;
    if (v14 < 0)
        v14 = 0;
    CheckGlobalPoint(this, AutoBuy, ChouJiangTimes, static_cast<int8_t>(v14), ChouJiangType);
    return 0;
}

int8_t ChouJiang::GetChouJiangType()
{
    int8_t type = Answer::Singleton<COpenBeta>::instance()->GetChouJiangType();
    if (type > 0)
        return type;
    type = Answer::Singleton<CUniteServer>::instance()->GetChouJiangType();
    if (type > 0)
        return type;
    type = Answer::Singleton<CFestivalDoubleEleven>::instance()->GetChouJiangType();
    if (type > 0)
        return type;
    return 1;
}

int32_t ChouJiang::GetChouJiangSpecialTime()
{
    int32_t time = Answer::Singleton<CUniteServer>::instance()->GetChouJiangSpecialTime();
    if (time > 0)
        return time;
    time = Answer::Singleton<CFestivalDoubleEleven>::instance()->GetChouJiangSpecialTime();
    if (time > 0)
        return time;
    return 0;
}

int32_t ChouJiang::OnChouJiang(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t AutoBuy = inPacket->readInt8();
    int8_t ChouJiangTimes = inPacket->readInt8();
    int8_t Special = inPacket->readInt8();
    int8_t ChouJiangType = inPacket->readInt8();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const ChouJiangTable* CouJiangTable = CfgData::GetCouJiangTable(cfg);
    const ChouJiangCost* pCost = ChouJiangTable::GetChouJiangCost(CouJiangTable, ChouJiangTimes);
    if (!pCost || pCost->Items.m_nCount <= 0 || pCost->Items.m_nClass <= 0 || pCost->Items.m_nId <= 0 || pCost->ItemPrice <= 0)
        return 2;
    if (GetFreeCount() < ChouJiangTimes)
        return 2;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    int32_t HaveAllCount = Bag->GetItemCount(pCost->Items.m_nId, pCost->Items.m_nClass);
    int32_t CostUnBindCount = 0;
    if (ChouJiangTimes <= HaveAllCount)
    {
        ItemData item;
        item.m_nId = pCost->Items.m_nId;
        item.m_nClass = pCost->Items.m_nClass;
        item.m_nCount = ChouJiangTimes;
        int32_t BindCount = 0, UnBindCount = 0;
        if (!Bag->RemoveItem(&item, ITEM_CHANGE_REASON::ICR_CHOU_JIANG, &BindCount, &UnBindCount))
            return 2;
        CostUnBindCount = UnBindCount;
    }
    else
    {
        if (AutoBuy != 1)
            return 2;
        if (Player::GetBag(m_pPlayer)->GetFreeSlotCount() <= 0)
        {
            Player::TiShiInfo(m_pPlayer, 2048, 0);
            return 2;
        }
        int32_t diffCount = ChouJiangTimes - HaveAllCount;
        const CfgGameShop* pCfgGameShop = CfgData::GetGameShopItem(cfg, pCost->Items.m_nClass, pCost->Items.m_nId);
        if (!pCfgGameShop || !Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, diffCount * pCfgGameShop->Price, CURRENCY_CHANGE_REASON::GCR_CHOU_JIANG, 0))
            return 2;
        int32_t BindCount = 0, UnBindCount = 0;
        if (HaveAllCount > 0)
        {
            ItemData item;
            item.m_nId = pCost->Items.m_nId;
            item.m_nClass = pCost->Items.m_nClass;
            item.m_nCount = HaveAllCount;
            if (!Bag->RemoveItem(&item, ITEM_CHANGE_REASON::ICR_CHOU_JIANG, &BindCount, &UnBindCount))
                return 2;
        }
        CostUnBindCount = diffCount + UnBindCount;
        MemChrBag stu;
        memset(&stu, 0, sizeof(stu));
        stu = pCost->Gift;
        stu.itemCount = diffCount;
        Bag->AddItem(&stu, ITEM_CHANGE_REASON::IDCR_CHOU_JIANG_BUY);
    }
    Player::AddCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_DRAW_SCORE, ChouJiangTimes, CURRENCY_CHANGE_REASON::SCR_ON_CHOU_JIANG, 0);
    Answer::Singleton<CKiaFuRecharge>::instance()->OnAddChouJiangTimes(m_pPlayer, ChouJiangTimes);
    Answer::Singleton<CUniteServer>::instance()->OnAddChouJiangTimes(m_pPlayer, ChouJiangTimes);
    Player::updateRecord(m_pPlayer, 1060, Player::getRecord(m_pPlayer, 1060) + ChouJiangTimes);
    Player::GetCNationalDayHd(m_pPlayer)->AddNationalValue(NATIONAL_HD_TYPE::HHT_CHOU_JIANG, ChouJiangTimes, 0);
    MemChrBagVector GetItmes;
    while (ChouJiangTimes > 0)
    {
        --ChouJiangTimes;
        int32_t IsFirst = Player::getRecord(m_pPlayer, 1059);
        if (IsFirst > 0 && Special <= 0)
        {
            int32_t SpecialTime = Answer::Singleton<CFestivalDoubleEleven>::instance()->GetChouJiangSpecialTime();
            if (SpecialTime > 0 && !(IsFirst % SpecialTime))
                Special = 4;
        }
        Player::GetOperateLimit(m_pPlayer)->AddLimitCount(1059, 1);
        int32_t KaiFuDay = CfgData::getServerDiffDay(cfg, SERVER_TYPE::SVT_NORMAL) + 1;
        int8_t v26 = ChouJiangType;
        int32_t m_LuckyPoint = this->m_LuckyPoint;
        bool v28 = IsFirst <= 0;
        const ChouJiangCfg* pChouJiangCfg = ChouJiangTable::RandItem(CouJiangTable, Special, v28, m_LuckyPoint, KaiFuDay, v26);
        if (pChouJiangCfg)
        {
            Special = 0;
            if (pChouJiangCfg->SubLuckValue > 0)
            {
                this->m_LuckyPoint -= pChouJiangCfg->SubLuckValue;
                if (this->m_LuckyPoint < -200)
                    this->m_LuckyPoint = -200;
            }
            if (pChouJiangCfg->JifenDel > 0)
                Player::updateRecord(m_pPlayer, 1059, 1);
            int32_t Time = Unit::getNow(m_pPlayer);
            MemChrBag stu;
            memset(&stu, 0, sizeof(stu));
            stu.itemClass = pChouJiangCfg->Item.itemClass;
            stu.itemId = pChouJiangCfg->Item.itemId;
            stu.itemCount = pChouJiangCfg->Item.itemCount;
            if (CostUnBindCount <= 0)
                stu.bind = stu.itemClass != 2;
            else
            {
                --CostUnBindCount;
                if (++this->m_LuckyPoint > 300)
                    this->m_LuckyPoint = 300;
            }
            if (pChouJiangCfg->GongGaoId > 0)
                GongGao(pChouJiangCfg->GongGaoId, pChouJiangCfg->Index, pChouJiangCfg->Item.itemClass, pChouJiangCfg->Item.itemId);
            if (pChouJiangCfg->PersonalRecord > 0)
                AddChouJiangRecord(&stu, Time);
            if (pChouJiangCfg->ServerRecord > 0)
                AddServerRecord(&stu);
            GetItmes.push_back(stu);
        }
    }
    SendWeekRewardInfo();
    AddItem(&GetItmes);
    return 0;
}

int32_t ChouJiang::OnAskChouJiangRecord(Answer::NetPacket* inPacket)
{
    SendChouJiangRecord();
    ToGlobalAskChouJiangRecord();
    SendWeekRewardInfo();
    return 0;
}

int32_t ChouJiang::ToGlobalAskChouJiangRecord()
{
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_PROC, 0x4EB8);
    if (!packet || !m_pPlayer)
        return 2;
    packet->writeInt32(Player::getGateIndex(m_pPlayer));
    packet->writeInt8(0);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService::sendPacket(gs, connId2, packet);
    return 0;
}

int32_t ChouJiang::OnGetItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t Slot = inPacket->readInt32();
    if (Slot < -1 || Slot > 489)
        return 2;
    if (Player::GetBag(m_pPlayer)->GetFreeSlotCount() <= 0)
        return 2;
    if (Slot == -1)
    {
        for (int i = 0; i <= 489; ++i)
        {
            if (m_ItemList[i].itemCount > 0 && m_ItemList[i].itemId > 0)
            {
                if (Player::GetBag(m_pPlayer)->GetFreeSlotCount() <= 0)
                {
                    Player::TiShiInfo(m_pPlayer, 2048, 0);
                    return 2;
                }
                GetItem(i);
            }
        }
    }
    else
    {
        GetItem(Slot);
    }
    return 0;
}

bool ChouJiang::AddItem(MemChrBagVector* p_Items)
{
    if (!m_pPlayer)
        return false;
    int32_t time = Unit::getNow(m_pPlayer);
    std::list<int> ItemList;
    for (auto& item : *p_Items)
    {
        for (int i = 0; i <= 489; ++i)
        {
            if (m_ItemList[i].itemCount <= 0 || m_ItemList[i].itemId <= 0)
            {
                m_ItemList[i] = item;
                ItemList.push_back(i);
                break;
            }
        }
    }
    SendChouJiangItemChang(&ItemList);
    SendGetItem(&ItemList);
    return true;
}

void ChouJiang::GetItem(int32_t Slot)
{
    if (!m_pPlayer || static_cast<uint32_t>(Slot) > 0x1E9)
        return;
    if (m_ItemList[Slot].itemCount <= 0 || m_ItemList[Slot].itemId <= 0)
        return;
    if (Player::GetBag(m_pPlayer)->GetFreeSlotCount() <= 0)
        return;
    MemChrBag BagItem = m_ItemList[Slot];
    MemChrBagVector item;
    item.push_back(BagItem);
    if (Player::GetBag(m_pPlayer)->AddItem(&item, ITEM_CHANGE_REASON::ICR_DRAW_GET_REWARD))
    {
        memset(&m_ItemList[Slot], 0, sizeof(MemChrBag));
        std::list<int> TmpList;
        TmpList.push_back(Slot);
        SendChouJiangItemChang(&TmpList);
    }
}

int32_t ChouJiang::GetFreeCount()
{
    int32_t FreeCount = 0;
    for (int i = 0; i <= 489; ++i)
    {
        if (m_ItemList[i].itemId <= 0 || m_ItemList[i].itemCount <= 0)
            ++FreeCount;
    }
    return FreeCount;
}

void ChouJiang::GetChouJiangStu(IconStateList* const IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 177))
    {
        ShowIcon icon;
        GetShowIconStu(&icon);
        IconList->push_back(icon);
    }
}

void ChouJiang::GetShowIconStu(ShowIcon* __return_ptr retstr)
{
    retstr->nId = 0;
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
    retstr->nId = 29;
    retstr->nState = 2;
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
}

void ChouJiang::SendHuoDongIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 177))
    {
        ShowIcon stu;
        GetShowIconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}

void ChouJiang::CleanBag()
{
    std::list<int> TmpList;
    for (int i = 0; i <= 489; ++i)
    {
        if (m_ItemList[i].itemCount > 0 && m_ItemList[i].itemId > 0)
        {
            memset(&m_ItemList[i], 0, sizeof(MemChrBag));
            TmpList.push_back(i);
        }
    }
    SendChouJiangItemChang(&TmpList);
    SendGetItem(&TmpList);
}

// ==================== 排序比较器 ====================

static bool GreaterChouJiangItem(const MemChrBag& left, const MemChrBag& right)
{
    if (left.itemClass < right.itemClass)
        return true;
    if (left.itemClass == right.itemClass)
        return left.itemId < right.itemId;
    return false;
}

// ==================== 网络协议处理 ====================

int32_t ChouJiang::OnTidy(Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    // 收集所有物品，合并同类项
    std::vector<MemChrBag> items;
    items.reserve(490);
    for (int i = 0; i < 490; ++i)
    {
        if (m_ItemList[i].itemCount > 0 && m_ItemList[i].itemId > 0)
        {
            bool bFind = false;
            for (auto& existing : items)
            {
                if (existing.itemClass == m_ItemList[i].itemClass &&
                    existing.itemId == m_ItemList[i].itemId &&
                    existing.bind == m_ItemList[i].bind &&
                    existing.srcId == m_ItemList[i].srcId)
                {
                    existing.itemCount += m_ItemList[i].itemCount;
                    bFind = true;
                    break;
                }
            }
            if (!bFind)
            {
                items.push_back(m_ItemList[i]);
            }
        }
    }
    // 排序
    std::sort(items.begin(), items.end(), GreaterChouJiangItem);
    // 重新分配回背包槽位
    int32_t slot = 0;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    for (auto& bagSlot : items)
    {
        if (slot >= 490)
            break;
        int32_t overlay = CfgData::getOverlay(cfg, bagSlot.itemId, bagSlot.itemClass);
        if (overlay > 0)
        {
            while (bagSlot.itemCount > overlay && slot < 490)
            {
                MemChrBag nullobj = bagSlot;
                nullobj.itemCount = overlay;
                bagSlot.itemCount -= overlay;
                m_ItemList[slot] = nullobj;
                ++slot;
            }
            if (bagSlot.itemCount > 0 && slot < 490)
            {
                m_ItemList[slot] = bagSlot;
                ++slot;
            }
        }
    }
    // 清空剩余槽位
    while (slot < 490)
    {
        memset(&m_ItemList[slot], 0, sizeof(MemChrBag));
        ++slot;
    }
    SendChouJiangItem();
    return 0;
}

int32_t ChouJiang::OnGetWeekReward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t Index = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const ChouJiangTable* CouJiangTable = CfgData::GetCouJiangTable(cfg);
    const ChouJiangWeekReward* pCfg = ChouJiangTable::GetChouJiangWeekReward(CouJiangTable, Index);
    if (!pCfg)
        return 10002;
    if (Player::getRecord(m_pPlayer, 1060) < pCfg->Times)
        return 10002;
    int32_t OldRecord = m_WeekReward;
    int32_t NewRecord = OldRecord | (1 << (Index - 1));
    if (OldRecord == NewRecord)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pCfg->Items, ITEM_CHANGE_REASON::ICR_CHOU_JIANG_WEEK_REWARD))
        return 2;
    m_WeekReward = NewRecord;
    SendWeekRewardInfo();
    return 0;
}

void ChouJiang::SendWeekRewardInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD2u);
    if (!packet)
        return;
    int32_t Record = Player::getRecord(m_pPlayer, 1060);
    packet->writeInt32(Record);
    packet->writeInt32(m_WeekReward);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

void ChouJiang::SendChouJiangRecord()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CC9u);
    if (!packet)
        return;
    int32_t Count = m_RecordList.size();
    packet->writeInt32(Count);
    for (auto& record : m_RecordList)
    {
        packet->writeInt32(record.CJItem.itemId);
        packet->writeInt8(record.CJItem.itemClass);
        packet->writeInt32(record.CJItem.itemCount);
        packet->writeInt32(record.Time);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

void ChouJiang::AddChouJiangRecord(MemChrBag* const CJItem, int32_t Time)
{
    ChouJiangRecord Record;
    memset(&Record, 0, sizeof(Record));
    Record.CJItem = *CJItem;
    Record.Time = Time;
    if (m_RecordList.size() > 49)
        m_RecordList.pop_front();
    m_RecordList.push_back(Record);
    SendNewRecord(CJItem, Time);
}

void ChouJiang::AddServerRecord(MemChrBag* const CJItem)
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_PROC, 0x4EB5u);
    if (!packet)
        return;
    int32_t GateIndex = Player::getGateIndex(m_pPlayer);
    packet->writeInt32(GateIndex);
    CharId_t Cid = Player::getCid(m_pPlayer);
    packet->writeInt64(Cid);
    MString name;
    Player::getPlayerName(m_pPlayer, &name);
    packet->writeUTF8(name);
    packet->writeInt8(CJItem->itemClass);
    packet->writeInt32(CJItem->itemId);
    packet->writeInt32(CJItem->itemCount);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    GameService::sendPacket(gs, Player::getConnId(m_pPlayer), packet);
}

void ChouJiang::SendNewRecord(MemChrBag* const CJItem, int32_t Time)
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CCAu);
    if (!packet)
        return;
    packet->writeInt32(CJItem->itemId);
    packet->writeInt8(CJItem->itemClass);
    packet->writeInt32(CJItem->itemCount);
    packet->writeInt32(Time);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

void ChouJiang::SendChouJiangItemChang(std::list<int>* const ItemList)
{
    if (!ItemList || ItemList->empty() || !m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CCFu);
    if (!packet)
        return;
    packet->writeInt32(static_cast<int32_t>(ItemList->size()));
    for (int Slot : *ItemList)
    {
        packet->writeInt32(Slot);
        packet->writeInt32(m_ItemList[Slot].itemId);
        packet->writeInt8(m_ItemList[Slot].itemClass);
        packet->writeInt32(m_ItemList[Slot].itemCount);
        packet->writeInt8(m_ItemList[Slot].bind);
        packet->writeInt32(m_ItemList[Slot].endTime);
        packet->writeInt64(m_ItemList[Slot].srcId);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

void ChouJiang::SendGetItem(std::list<int>* const ItemList)
{
    if (!ItemList || ItemList->empty() || !m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD1u);
    if (!packet)
        return;
    packet->writeInt32(static_cast<int32_t>(ItemList->size()));
    for (int Slot : *ItemList)
    {
        packet->writeInt32(Slot);
        packet->writeInt32(m_ItemList[Slot].itemId);
        packet->writeInt8(m_ItemList[Slot].itemClass);
        packet->writeInt32(m_ItemList[Slot].itemCount);
        packet->writeInt8(m_ItemList[Slot].bind);
        packet->writeInt32(m_ItemList[Slot].endTime);
        packet->writeInt64(m_ItemList[Slot].srcId);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

void ChouJiang::SendChouJiangItem()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CCEu);
    if (!packet)
        return;
    int32_t Count = 0;
    uint32_t CountPos = packet->getWOffset();
    packet->writeInt32(0);
    for (int i = 0; i < 490; ++i)
    {
        if (m_ItemList[i].itemId > 0 && m_ItemList[i].itemCount > 0)
        {
            ++Count;
            packet->writeInt32(i);
            packet->writeInt32(m_ItemList[i].itemId);
            packet->writeInt8(m_ItemList[i].itemClass);
            packet->writeInt32(m_ItemList[i].itemCount);
            packet->writeInt8(m_ItemList[i].bind);
            packet->writeInt32(m_ItemList[i].endTime);
            packet->writeInt64(m_ItemList[i].srcId);
        }
    }
    uint32_t EndPos = packet->getWOffset();
    packet->setWOffset(CountPos);
    packet->writeInt32(Count);
    packet->setWOffset(EndPos);
    packet->setSize(EndPos);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

