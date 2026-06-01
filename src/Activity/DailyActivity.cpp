#include "Activity/DailyActivity.h"
#include "Activity/CActivityManager.h"
#include "Config/CfgData.h"
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Game/CTimer.h"
#include "Database/DBService.h"
#include "Character/CExtCharBag.h"
#include "Other/CVip.h"
#include "Other/CHuoYueDu.h"
#include "Other/CFunctionOpen.h"
#include "Character/CExtChrTaskCycle.h"
#include "Character/CExtCharFamily.h"
#include "Other/ChouJiang.h"
#include "Utility/StringUtility.h"
#include "Other/CVplan.h"
#include "Other/CEquipManager.h"
#include "Other/Logger.h"
#include "Other/DayTime.h"
#include "Other/Answer.h"

DailyActivity::DailyActivity()
    : m_LastUpdate(0)
    , m_LastUpdateIcon(0)
    , m_IsLogin(0)
    , m_pPlayer(nullptr)
{
}

DailyActivity::~DailyActivity()
{
    m_vSeachBackRecord[2].clear();
    m_vSeachBackRecord[1].clear();
    m_vSeachBackRecord[0].clear();
    m_lstTodayInfo.clear();
    m_OnlimeTimeGetReward.clear();
}

void DailyActivity::AddActivityRecord(int32_t nType, int32_t nId)
{
    if (!m_pPlayer) return;
    
    CHuoYueDu* PlayerHuoYueDu = m_pPlayer->GetPlayerHuoYueDu();
    PlayerHuoYueDu->AddHuoYueDuRecord(1, nType, 0);
    
    for (auto& info : m_lstTodayInfo)
    {
        if (info.nType == 1 && info.nSubType == nType)
        {
            if (info.nParam != nId)
            {
                info.nTimes++;
                info.nParam = nId;
            }
            return;
        }
    }
    
    SearchBackInfo stu;
    stu.nType = 1;
    stu.nSubType = nType;
    stu.nTimes = 1;
    stu.nParam = nId;
    m_lstTodayInfo.push_back(stu);
}

void DailyActivity::AddDungeonRecord(int32_t nType, int32_t nTimes, int32_t nParam)
{
    if (!m_pPlayer) return;
    
    for (auto& info : m_lstTodayInfo)
    {
        if (info.nType == 2 && info.nSubType == nType)
        {
            info.nTimes = nTimes;
            info.nParam = nParam;
            return;
        }
    }
    
    SearchBackInfo stu;
    stu.nType = 2;
    stu.nSubType = nType;
    stu.nTimes = nTimes;
    stu.nParam = nParam;
    m_lstTodayInfo.push_back(stu);
}

void DailyActivity::AddTaskRecord(int32_t nType)
{
    if (!m_pPlayer) return;
    
    for (auto& info : m_lstTodayInfo)
    {
        if (info.nType == 3 && info.nSubType == nType)
        {
            info.nTimes++;
            info.nParam = 0;
            return;
        }
    }
    
    SearchBackInfo stu;
    stu.nType = 3;
    stu.nSubType = nType;
    stu.nTimes = 1;
    stu.nParam = 0;
    m_lstTodayInfo.push_back(stu);
}

void DailyActivity::DispatchNetDatas(Answer::NetPacket* inPacket, uint16_t nProcId)
{
    if (!inPacket) return;
    
    switch (nProcId)
    {
    case 73:  // 'I'
        OnQuerySignInfo(inPacket);
        break;
    case 74:  // 'J'
        OnSign(inPacket);
        break;
    case 75:  // 'K'
        OnGetSignReward(inPacket);
        break;
    case 154:
        OnGetOnLineReward(inPacket);
        break;
    case 155:
        OnQueryOnLineInfo(inPacket);
        break;
    case 156:
        OnGetWeekOnLineReward(inPacket);
        break;
    case 202:
        OnGetSevenLoginRewrad(inPacket);
        break;
    case 203:
        OnQuerySevenLoginInfo(inPacket);
        break;
    case 459:
        OnQueryOffLineExpInfo(inPacket);
        break;
    case 460:
        OnGetOffLineExp(inPacket);
        break;
    case 462:
        OnQueryHuoDaoDaTingData(inPacket);
        break;
    case 475:
        OnGetSignDailyReward(inPacket);
        break;
    case 476:
        OnGetSearchBackReward(inPacket);
        break;
    case 895:
        OnGetMaintainCompensateReward(inPacket);
        break;
    case 992:
        OnGetDogTitle(inPacket);
        break;
    case 993:
        OnAskDogTitleInfo(inPacket);
        break;
    case 20081:
        RecordEnterNumber(inPacket);
        break;
    default:
        break;
    }
}

void DailyActivity::GetHuoDongDaTingIcon(std::list<ShowIcon>& IconList)
{
    if (!m_pPlayer) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (PlayerFunctionOpen->IsOpened(59))
    {
        ShowIcon icon;
        GetHuoDongDaTingIcon(&icon);
        IconList.push_back(icon);
    }
}

void DailyActivity::GetInterestsProtocol(std::list<uint16_t>& procList)
{
    procList.push_back(73);
    procList.push_back(74);
    procList.push_back(75);
    procList.push_back(475);
    procList.push_back(154);
    procList.push_back(155);
    procList.push_back(156);
    procList.push_back(202);
    procList.push_back(203);
    procList.push_back(459);
    procList.push_back(460);
    procList.push_back(462);
    procList.push_back(476);
    procList.push_back(20081);
    procList.push_back(895);
    procList.push_back(992);
    procList.push_back(993);
}

void DailyActivity::GetJiangLiDaTingIcon(std::list<ShowIcon>& IconList)
{
    if (!m_pPlayer) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (PlayerFunctionOpen->IsOpened(60))
    {
        ShowIcon stu;
        GetJiangLiDaTingIcon(&stu);
        IconList.push_back(stu);
    }
}

int32_t DailyActivity::GetLastWeekOnlineTime()
{
    if (!m_pPlayer) return 0;
    return m_pPlayer->getRecord(1017);
}

int32_t DailyActivity::GetNewSeverWeek()
{
    CfgData* cfgData = CfgData::GetInstance();
    int32_t ServerStartTime = cfgData->GetServerStartTime(SERVER_TYPE::SVT_NORMAL);
    return DayTime::weekdiff(ServerStartTime) + 1;
}

void DailyActivity::GetSearchBackIcon(std::list<ShowIcon>& IconList)
{
    if (!m_pPlayer) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (PlayerFunctionOpen->IsOpened(168))
    {
        ShowIcon stu;
        GetSearchBackIcon(&stu);
        IconList.push_back(stu);
    }
}

bool DailyActivity::GetSearchReward(const SearchBackRecord& record, int8_t nFree, int32_t Times)
{
    if (!m_pPlayer) return false;
    
    if (record.nTimes < Times) return false;
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgSearchBackTable* SearchBackTable = cfgData->GetSearchBackTable();
    const CfgSearchBack* pCfgSearchBack = SearchBackTable->GetSearchBack(record.nId);
    
    if (!pCfgSearchBack) return false;
    
    int32_t nGoldCost = Times * pCfgSearchBack->nGoldCost;
    
    std::vector<MemChrBag> vItemReward;
    vItemReward = pCfgSearchBack->vFreeReward;
    
    if (nFree > 0)
    {
        if (nGoldCost > 0)
        {
            int64_t Currency = m_pPlayer->GetCurrency(CURRENCY_TYPE::CURRENCY_GOLD);
            if (Currency < nGoldCost)
            {
                m_pPlayer->TiShiInfo(11, 0);
                return false;
            }
        }
        vItemReward = pCfgSearchBack->vItemReward;
    }
    
    if (vItemReward.empty()) return false;
    
    for (auto& item : vItemReward)
    {
        item.itemCount *= Times;
    }
    
    CExtCharBag* Bag = m_pPlayer->GetBag();
    if (!Bag->AddItem(&vItemReward, ITEM_CHANGE_REASON::ICR_SEARCH_BACK))
    {
        m_pPlayer->TiShiInfo(2048, 0);
        return false;
    }
    
    if (nFree > 0 && nGoldCost > 0)
    {
        m_pPlayer->DecCurrency(CURRENCY_TYPE::CURRENCY_GOLD, nGoldCost,
                               CURRENCY_CHANGE_REASON::MCR_SEARCH_BACK, 0);
    }
    
    UpdateSearchBackRecord(record.nId, Times);
    return true;
}

void DailyActivity::GetSevenDayLoginIcon(std::list<ShowIcon>& IconList)
{
    if (!m_pPlayer) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (PlayerFunctionOpen->IsOpened(112))
    {
        ShowIcon stu;
        GetSevenDayLoginIcon(&stu);
        IconList.push_back(stu);
    }
}

int32_t DailyActivity::GetTodayOnLineTime()
{
    if (!m_pPlayer) return 0;
    return m_pPlayer->getRecord(2003);
}

int32_t DailyActivity::GetWeekOnlineTime()
{
    if (!m_pPlayer) return 0;
    return m_pPlayer->getRecord(1016);
}

int32_t DailyActivity::HaveActivityRewardCount()
{
    int32_t Count = 0;
    std::list<int32_t> ActivityId;
    
    // 获取需要检查的活动ID列表
    // ActivityId = GetActivityIdList();
    
    for (int32_t v1 : ActivityId)
    {
        CActivityManager* activityMgr = CActivityManager::GetInstance();
        int32_t HaveRewardCount = activityMgr->HaveRewardCount(m_pPlayer, v1);
        Count += HaveRewardCount;
    }
    
    return Count;
}

bool DailyActivity::HaveEquipShow()
{
    if (!m_pPlayer) return false;
    
    int32_t Level = m_pPlayer->GetLevel();
    return Level > 89 && Level <= 360;
}

bool DailyActivity::HaveLevelReward()
{
    if (!m_pPlayer) return false;
    
    int32_t Level = m_pPlayer->GetLevel();
    int32_t RewardRecord = m_pPlayer->getRecord(1015);
    
    CfgData* cfgData = CfgData::GetInstance();
    const std::map<int, CfgLevelGift>* LevelGiftTable = cfgData->GetLevelGiftTable();
    
    for (const auto& pair : *LevelGiftTable)
    {
        if (pair.second.Level <= Level)
        {
            if (((RewardRecord >> (pair.first - 1)) & 1) == 0)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool DailyActivity::HaveMaintainCompensateReward()
{
    if (!m_pPlayer) return false;
    
    CExtOperateLimit* OperateLimit = m_pPlayer->GetOperateLimit();
    int32_t v3 = OperateLimit->GetLimitCount(1070) + 1;
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgMaintainCompensateTable* MaintainCompensateTable = cfgData->GetMaintainCompensateTable();
    const CfgMaintainCompensate* pCfg = MaintainCompensateTable->GetInfo(v3);
    
    if (pCfg)
    {
        int32_t nTime = pCfg->nTime;
        if (nTime <= m_pPlayer->getNow())
        {
            return true;
        }
    }
    
    return false;
}

bool DailyActivity::HaveOffLineReard()
{
    if (!m_pPlayer) return false;
    return m_pPlayer->getRecord(1019) > 3600;
}

bool DailyActivity::HaveOnLineReward()
{
    if (!m_pPlayer) return false;
    
    int32_t TodayTime = GetTodayOnLineTime();
    int32_t RewardRecord = m_pPlayer->getRecord(2004);
    
    CfgData* cfgData = CfgData::GetInstance();
    const std::map<int8_t, CfgOnlineReward>* OnLineRewardTable = cfgData->GetOnlineRewardTable();
    
    for (const auto& pair : *OnLineRewardTable)
    {
        if (pair.second.NeedTime <= TodayTime)
        {
            if (((RewardRecord >> (pair.first - 1)) & 1) == 0)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool DailyActivity::HaveSignReward()
{
    if (!m_pPlayer) return false;
    
    tm tmNow;
    m_pPlayer->getLocalNow(&tmNow);
    
    int32_t signCount = 0;
    for (int i = 0; i < tmNow.tm_mday; ++i)
    {
        signCount += (m_signInfo.sign_record >> i) & 1;
    }
    
    CfgData* cfgData = CfgData::GetInstance();
    const std::map<int8_t, CfgSignReward>* ReardTable = cfgData->GetSignRewardTable();
    
    for (const auto& pair : *ReardTable)
    {
        if (pair.second.count <= signCount)
        {
            bool IsGet = false;
            for (int8_t record : m_signInfo.sign_reward)
            {
                if (record == pair.second.count)
                {
                    IsGet = true;
                    break;
                }
            }
            if (!IsGet)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool DailyActivity::HaveWeekReward()
{
    return GetNewSeverWeek() > 1 && GetLastWeekOnlineTime() > 3599;
}

void DailyActivity::OnAskDogTitleInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return;
    
    int8_t nType = inPacket->readInt8();
    
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x2CE3);
    
    if (!packet) return;
    
    packet->writeInt8(nType);
    packet->writeInt32(m_pPlayer->getRecord(1158));
    packet->writeInt32(m_pPlayer->getRecord(1159));
    packet->setSize(packet->getWOffset());
    
    gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
}

void DailyActivity::OnCleanUp()
{
    memset(&m_signInfo, 0, sizeof(m_signInfo));
    m_LastUpdate = 0;
    m_LastUpdateIcon = 0;
    m_IsLogin = 0;
    m_lstTodayInfo.clear();
    
    for (int i = 0; i <= 2; ++i)
    {
        m_vSeachBackRecord[i].clear();
    }
    
    m_OnlimeTimeGetReward.clear();
}

void DailyActivity::OnDaySwitch()
{
    m_OnlimeTimeGetReward.clear();
    RefreshSignInfo();
    RefreshWeekTime();
    RefreshSearchBack(1);
    SendJiangLiDaTingIcon();
    SendSearchBackIcon();
    SendSevenDayLoginIcon();
    SendSearchBackInfo();
}

void DailyActivity::OnGetDogTitle(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return;
    
    int8_t Type = inPacket->readInt8();
    
    MemChrBag stu;
    memset(&stu, 0, sizeof(stu));
    
    if (Type == 1)
    {
        if (m_pPlayer->getRecord(1158) > 0) return;
        
        stu.itemId = 0x14DF;  // 5335
        stu.itemClass = 1;
        stu.itemCount = 1;
        
        CExtCharBag* Bag = m_pPlayer->GetBag();
        if (Bag->AddItem(&stu, ITEM_CHANGE_REASON::IDCR_DOG_TITLE_GET))
        {
            m_pPlayer->updateRecord(1158, 1);
        }
    }
    else if (Type == 2)
    {
        if (m_pPlayer->getRecord(1159) > 0) return;
        
        stu.itemId = 0x1B95;  // 7061
        stu.itemClass = 1;
        stu.itemCount = 1;
        
        CExtCharBag* Bag = m_pPlayer->GetBag();
        if (Bag->AddItem(&stu, ITEM_CHANGE_REASON::IDCR_DOG_TITLE_GET))
        {
            m_pPlayer->updateRecord(1159, 1);
        }
    }
}

void DailyActivity::OnGetLevelGift(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return;
    
    uint8_t Index = inPacket->readUInt8();
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgLevelGift* pLevelGift = cfgData->GetLevelGift(Index);
    
    if (!pLevelGift) return;
    
    if (pLevelGift->Level > m_pPlayer->GetLevel()) return;
    
    int32_t OldRecord = m_pPlayer->getRecord(1015);
    int32_t NewRecord = OldRecord | (1 << (Index - 1));
    
    if (OldRecord == NewRecord) return;
    
    CExtCharBag* Bag = m_pPlayer->GetBag();
    if (!Bag->AddItem(&pLevelGift->ItemVector, ITEM_CHANGE_REASON::ICR_SEVEN_LOGIN_REWARD)) return;
    
    m_pPlayer->updateRecord(1015, NewRecord);
    SendLevelGiftInfo();
    SendJiangLiDaTingIcon();
}

void DailyActivity::OnGetMaintainCompensateReward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return;
    
    int32_t nIndex = inPacket->readInt32();
    
    CExtOperateLimit* OperateLimit = m_pPlayer->GetOperateLimit();
    if (OperateLimit->GetLimitCount(1070) + 1 != nIndex) return;
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgMaintainCompensateTable* MaintainCompensateTable = cfgData->GetMaintainCompensateTable();
    const CfgMaintainCompensate* pCfg = MaintainCompensateTable->GetInfo(nIndex);
    
    if (!pCfg) return;
    
    if (pCfg->nTime > m_pPlayer->getNow()) return;
    
    CExtCharBag* Bag = m_pPlayer->GetBag();
    if (!Bag->AddItem(&pCfg->vItems, ITEM_CHANGE_REASON::ICR_MAINTAIN_COMPENSATE_REWARD)) return;
    
    OperateLimit->UpdateLimitCount(1070, nIndex);
    SendJiangLiDaTingIcon();
}

void DailyActivity::OnGetOffLineExp(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return;
    
    int8_t Times = inPacket->readInt8();
    
    int32_t Level = m_pPlayer->GetLevel();
    CfgData* cfgData = CfgData::GetInstance();
    const CfgOfflineExp* pOfflineExp = cfgData->GetOfflineExp(Level);
    
    if (!pOfflineExp) return;
    
    int32_t OfflineSeconds = m_pPlayer->getRecord(1019);
    if (OfflineSeconds > 259200) OfflineSeconds = 259200;
    
    int32_t OfflineMinutes = OfflineSeconds / 60;
    
    double CanAddExp = static_cast<double>(OfflineMinutes * pOfflineExp->MinuteExp);
    int32_t NeedGold = 0;
    
    if (Times == 2)
    {
        NeedGold = static_cast<int32_t>(static_cast<double>(pOfflineExp->TwoTimes) * (CanAddExp / static_cast<double>(pOfflineExp->BaseExp)) / 100.0);
        CanAddExp = CanAddExp * 2;
    }
    else if (Times == 3)
    {
        NeedGold = static_cast<int32_t>(static_cast<double>(pOfflineExp->ThreeTimes) * (CanAddExp / static_cast<double>(pOfflineExp->BaseExp)) / 100.0);
        CanAddExp = CanAddExp * 3;
    }
    
    if (CanAddExp <= 0) return;
    
    if (NeedGold > 0)
    {
        if (!m_pPlayer->DecCurrency(CURRENCY_TYPE::CURRENCY_GOLD, NeedGold,
                                     CURRENCY_CHANGE_REASON::GCR_GET_OFFLINE_EXP, 0))
        {
            return;
        }
    }
    
    m_pPlayer->updateRecord(1019, 0);
    m_pPlayer->addExp(static_cast<uint32_t>(CanAddExp), 0, 1);
    SendJiangLiDaTingIcon();
}

void DailyActivity::OnGetOnLineReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return;
    
    int8_t Id = inPacket->readInt8();
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgOnlineReward* pReward = cfgData->GetOnlineReward(Id);
    
    if (!pReward) return;
    
    if (GetTodayOnLineTime() < pReward->NeedTime) return;
    
    int32_t oldRecord = m_pPlayer->getRecord(2004);
    int32_t NewRecord = oldRecord | (1 << (Id - 1));
    
    if (oldRecord == NewRecord) return;
    
    if (pReward->ItemVector.empty()) return;
    
    size_t nSize = pReward->ItemVector.size();
    Answer::Random* random = Answer::Random::GetInstance();
    int32_t nRand = random->generate(0, static_cast<int32_t>(nSize - 1));
    MemChrBag* v5 = pReward->ItemVector[nRand];
    
    CExtCharBag* Bag = m_pPlayer->GetBag();
    if (!Bag->AddItem(v5, ITEM_CHANGE_REASON::ICR_ONLINE_REWAR)) return;
    
    m_OnlimeTimeGetReward[Id] = nRand;
    m_pPlayer->updateRecord(2004, NewRecord);
    SendOnlineRewardInfo();
    SendJiangLiDaTingIcon();
}

void DailyActivity::OnGetSearchBackReward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (!PlayerFunctionOpen->IsOpened(168)) return;
    
    int32_t nId = inPacket->readInt32();
    int8_t nFree = inPacket->readInt8();
    
    if (nId <= 0)
    {
        for (int i = 2; i >= 0; --i)
        {
            std::list<SearchBackRecord> ItemListTmp = m_vSeachBackRecord[i];
            
            for (const auto& record : ItemListTmp)
            {
                if (GetSearchReward(record, nFree, record.nTimes))
                {
                    SendSearchBackInfo();
                    SendSearchBackIcon();
                }
            }
        }
    }
    else
    {
        SearchBackRecord record = GetSearchBackRecord(nId);
        GetSearchReward(record, nFree, 1);
        SendSearchBackInfo();
        SendSearchBackIcon();
    }
}

void DailyActivity::OnGetSevenLoginRewrad(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return;
    
    int8_t Day = inPacket->readInt8();
    
    if (Day > 7) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (!PlayerFunctionOpen->IsOpened(112)) return;
    
    int32_t LoginCount = m_pPlayer->getRecord(1011);
    if (Day > LoginCount) return;
    
    int32_t OldRecord = m_pPlayer->getRecord(1014);
    int32_t NewRecord = OldRecord | (1 << (Day - 1));
    
    if (OldRecord == NewRecord) return;
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgSevenLoginReward* pReward = cfgData->GetSevenLoginReward(Day);
    
    if (!pReward) return;
    
    std::vector<MemChrBag> addItemVt;
    std::vector<MemEquip> vEquip;
    
    for (const auto& cfgItem : pReward->ItemVector)
    {
        int32_t Job = m_pPlayer->GetJob();
        
        if (Job == cfgItem.job || cfgItem.job == 0)
        {
            MemChrBag chrbag;
            memset(&chrbag, 0, sizeof(chrbag));
            chrbag.itemId = cfgItem.id;
            chrbag.itemClass = cfgItem.type;
            chrbag.itemCount = cfgItem.count;
            chrbag.bind = cfgItem.bind;
            
            if (chrbag.itemClass == 2)
            {
                int32_t Time = cfgItem.star;
                std::string p_name = m_pPlayer->getName();
                int64_t nCreaterId = m_pPlayer->getCid();
                int32_t Sid = m_pPlayer->getSid();
                int8_t ConnId = m_pPlayer->getConnId();
                
                CEquipManager* equipMgr = CEquipManager::GetInstance();
                MemEquip equip;
                equipMgr->CreateMemEquip(&equip, ConnId, 601, chrbag.itemId, Sid, nCreaterId, &p_name, 0, 0, Time, 0, 0);
                
                if (equip.base > 0)
                {
                    chrbag.srcId = equip.id;
                    vEquip.push_back(equip);
                }
            }
            
            addItemVt.push_back(chrbag);
        }
    }
    
    CExtCharBag* Bag = m_pPlayer->GetBag();
    if (!Bag->AddItem(&addItemVt, ITEM_CHANGE_REASON::ICR_SEVEN_LOGIN_REWARD)) return;
    
    if (!vEquip.empty())
    {
        CEquipManager* equipMgr = CEquipManager::GetInstance();
        equipMgr->SendPlayerEquipInfo(m_pPlayer, &vEquip);
    }
    
    m_pPlayer->updateRecord(1014, NewRecord);
    SendSevenLoginInfo();
    SendSevenDayLoginIcon();
}

void DailyActivity::OnGetWeekOnLineReward(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    
    int32_t Week = GetNewSeverWeek();
    if (Week <= 1) return;
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgWeekOnlineReward* pWeekReward = cfgData->GetWeekOnlineReward(Week - 1);
    
    if (!pWeekReward) return;
    
    int32_t LastWeekOnLineHour = GetLastWeekOnlineTime() / 3600;
    if (LastWeekOnLineHour > pWeekReward->UpperLimit)
    {
        LastWeekOnLineHour = pWeekReward->UpperLimit;
    }
    
    if (LastWeekOnLineHour <= 0) return;
    
    int32_t GetCash = LastWeekOnLineHour * pWeekReward->Cash;
    if (GetCash <= 0) return;
    
    m_pPlayer->updateRecord(1017, 0);
    
    if (!m_pPlayer->AddCurrency(CURRENCY_TYPE::CURRENCY_CASH, GetCash,
                                CURRENCY_CHANGE_REASON::GCC_WEEK_ONLINE_REWARD, 0))
    {
        LOG_ERROR("WEEK ONLINE REWARD ERR Cid=%lld,time=%d,GetCash=%d",
                  m_pPlayer->getCid(), m_pPlayer->getNow(), GetCash);
    }
    
    SendOnlineRewardInfo();
    SendJiangLiDaTingIcon();
}

void DailyActivity::OnLevelUp(int32_t newLevel)
{
    if (HaveLevelReward())
    {
        SendJiangLiDaTingIcon();
    }
}

void DailyActivity::OnLoadFromDB(const DailyActivityDBData* dbData)
{
    m_signInfo = dbData->signInfo.data;
    m_lstTodayInfo = dbData->signInfo.lstTodayInfo;
    
    for (int i = 0; i <= 2; ++i)
    {
        m_vSeachBackRecord[i] = dbData->signInfo.lstTodayInfo2[i];
    }
    
    PasOnlineReward(dbData->signInfo.OnlimeReward);
}

void DailyActivity::OnQueryHuoDaoDaTingData(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    
    int8_t Int8 = inPacket->readInt8();
    SendHuoDaoDaTingData(Int8);
}

void DailyActivity::OnQueryLevelGiftInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    SendLevelGiftInfo();
}

void DailyActivity::OnQueryOffLineExpInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return;
    
    if (HaveOffLineReard())
    {
        SendOffLineInfo();
    }
    else
    {
        int8_t ConnId = m_pPlayer->getConnId();
        GameService* gameService = GameService::GetInstance();
        Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x27C4);
        if (packet)
        {
            packet->writeInt32(0);
            packet->setSize(packet->getWOffset());
            gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
        }
    }
}

void DailyActivity::OnQueryOnLineInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    SendOnlineRewardInfo();
}

void DailyActivity::OnQuerySevenLoginInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    SendSevenLoginInfo();
}

void DailyActivity::OnSaveToDB(DailyActivityDBData* dbData)
{
    dbData->signInfo.data = m_signInfo;
    dbData->signInfo.lstTodayInfo = m_lstTodayInfo;
    
    for (int i = 0; i <= 2; ++i)
    {
        dbData->signInfo.lstTodayInfo2[i] = m_vSeachBackRecord[i];
    }
    
    dbData->signInfo.OnlimeReward = GetOnlineRewardString();
}

void DailyActivity::OnUpdate(int64_t curTick)
{
    if (!m_pPlayer) return;
    
    if (curTick - m_LastUpdate > 999)
    {
        int32_t Record = m_pPlayer->getRecord(2003);
        m_pPlayer->updateRecord(2003, Record + 1);
        
        int32_t v3 = m_pPlayer->getRecord(1016);
        m_pPlayer->updateRecord(1016, v3 + 1);
        
        // CFestivalDoubleEleven::AddOnlineRecord(m_pPlayer);
        // COpenBeta::AddOnlineRecord(m_pPlayer);
        
        m_LastUpdate = curTick;
    }
    
    UpDateJiangLiDatingIcon(curTick);
}

void DailyActivity::PasOnlineReward(const std::string& p_OnlineReward)
{
    if (p_OnlineReward.empty()) return;
    
    m_OnlimeTimeGetReward.clear();
    
    std::vector<std::string> strstacks;
    StringUtility::split(strstacks, p_OnlineReward, "|");
    
    for (const auto& stack : strstacks)
    {
        std::vector<std::string> vstack;
        StringUtility::split(vstack, stack, ":|");
        
        if (vstack.size() == 2)
        {
            int32_t key = atoi(vstack[0].c_str());
            int32_t value = atoi(vstack[1].c_str());
            m_OnlimeTimeGetReward[key] = value;
        }
    }
}

void DailyActivity::RecordEnterNumber(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    
    int32_t ActivityId = inPacket->readInt32();
    int32_t Int32 = inPacket->readInt32();
    AddActivityRecord(Int32, ActivityId);
}

void DailyActivity::SendHuoDaoDaTingData(int8_t Type)
{
    if (!m_pPlayer) return;
    
    if (Type == 1)
    {
        int8_t ConnId = m_pPlayer->getConnId();
        GameService* gameService = GameService::GetInstance();
        Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x2CD8);
        
        if (packet)
        {
            int32_t nCount = 0;
            packet->writeInt8(1);
            
            int32_t nOldOffset = packet->getWOffset();
            packet->writeInt32(0);
            
            CActivityManager* activityMgr = CActivityManager::GetInstance();
            activityMgr->AppendActivityState(packet, &nCount);
            
            int32_t nNewOffset = packet->getWOffset();
            packet->setWOffset(nOldOffset);
            packet->writeInt32(nCount);
            packet->setWOffset(nNewOffset);
            packet->setSize(packet->getWOffset());
            
            gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
        }
    }
    else if (Type == 3)
    {
        int8_t ConnId = m_pPlayer->getConnId();
        GameService* gameService = GameService::GetInstance();
        Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x2CD8);
        
        if (packet)
        {
            packet->writeInt8(3);
            packet->writeInt32(3);
            packet->writeInt8(1);
            
            CExtChrTaskCycle* CharTaskCycle = m_pPlayer->GetCharTaskCycle();
            int32_t SurplusTimes = CharTaskCycle->GetSurplusTimes();
            packet->writeInt32(SurplusTimes);
            
            packet->writeInt8(2);
            int32_t Record = m_pPlayer->getRecord(2005);
            packet->writeInt32(2 - Record);
            
            packet->writeInt8(3);
            int32_t v14 = m_pPlayer->getRecord(2019);
            packet->writeInt32(10 - v14);
            
            packet->setSize(packet->getWOffset());
            gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
        }
    }
}

void DailyActivity::SendHuoDongDaTingIcon()
{
    if (!m_pPlayer) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (PlayerFunctionOpen->IsOpened(59))
    {
        ShowIcon icon;
        GetHuoDongDaTingIcon(&icon);
        m_pPlayer->SendIconState(&icon);
    }
}

void DailyActivity::SendJiangLiDaTingIcon()
{
    if (!m_pPlayer) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (PlayerFunctionOpen->IsOpened(60))
    {
        ShowIcon stu;
        GetJiangLiDaTingIcon(&stu);
        m_pPlayer->SendIconState(&stu);
    }
}

void DailyActivity::SendLevelGiftInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x27F4);
    
    if (packet)
    {
        int32_t Record = m_pPlayer->getRecord(1015);
        packet->writeInt32(Record);
        packet->setSize(packet->getWOffset());
        gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
    }
}

void DailyActivity::SendOffLineInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x27C4);
    
    if (packet)
    {
        int32_t OfflineSeconds = m_pPlayer->getRecord(1019);
        if (OfflineSeconds > 259200) OfflineSeconds = 259200;
        
        packet->writeInt32(OfflineSeconds);
        packet->setSize(packet->getWOffset());
        gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
    }
}

void DailyActivity::SendOnlineRewardInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x27AB);
    
    if (packet)
    {
        packet->writeInt32(GetTodayOnLineTime());
        packet->writeInt32(m_pPlayer->getRecord(2004));
        packet->writeInt32(GetNewSeverWeek());
        packet->writeInt32(GetWeekOnlineTime());
        packet->writeInt32(GetLastWeekOnlineTime());
        packet->writeInt32(static_cast<int32_t>(m_OnlimeTimeGetReward.size()));
        
        for (const auto& pair : m_OnlimeTimeGetReward)
        {
            packet->writeInt32(pair.first);
            packet->writeInt32(pair.second);
        }
        
        packet->setSize(packet->getWOffset());
        gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
    }
}

void DailyActivity::SendSearchBackInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x2CE2);
    
    if (packet)
    {
        packet->writeInt8(3);
        
        for (int i = 0; i <= 2; ++i)
        {
            packet->writeInt32(static_cast<int32_t>(m_vSeachBackRecord[i].size()));
            
            for (const auto& record : m_vSeachBackRecord[i])
            {
                packet->writeInt32(record.nId);
                packet->writeInt32(record.nTimes);
                packet->writeInt32(record.nParam);
            }
        }
        
        packet->setSize(packet->getWOffset());
        gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
    }
}

void DailyActivity::SendSevenDayLoginIcon()
{
    if (!m_pPlayer) return;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (PlayerFunctionOpen->IsOpened(112))
    {
        ShowIcon stu;
        GetSevenDayLoginIcon(&stu);
        m_pPlayer->SendIconState(&stu);
    }
}

void DailyActivity::SendSevenLoginInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x27FB);
    
    if (packet)
    {
        packet->writeInt32(m_pPlayer->getRecord(1011));
        packet->writeInt32(m_pPlayer->getRecord(1014));
        packet->setSize(packet->getWOffset());
        gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
    }
}

bool DailyActivity::TodayIsSign()
{
    if (!m_pPlayer) return false;
    
    tm tmNow;
    m_pPlayer->getLocalNow(&tmNow);
    
    return ((m_signInfo.sign_record >> (tmNow.tm_mday - 1)) & 1) != 0;
}

void DailyActivity::UpDateJiangLiDatingIcon(int64_t curTick)
{
    if (!m_pPlayer) return;
    
    if (curTick - m_LastUpdateIcon > 60000)
    {
        if (HaveOnLineReward())
        {
            SendJiangLiDaTingIcon();
        }
        m_LastUpdateIcon = curTick;
    }
}

bool DailyActivity::HaveSearchBackReward()
{
    if (!m_pPlayer) return false;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (!PlayerFunctionOpen->IsOpened(168)) return false;
    
    for (int i = 0; i <= 2; ++i)
    {
        if (!m_vSeachBackRecord[i].empty())
        {
            return true;
        }
    }
    
    return false;
}

bool DailyActivity::HaveSevenLoginReward()
{
    if (!m_pPlayer) return false;
    
    CFunctionOpen* PlayerFunctionOpen = m_pPlayer->GetPlayerFunctionOpen();
    if (!PlayerFunctionOpen->IsOpened(112)) return false;
    
    int32_t LoginCount = m_pPlayer->getRecord(1011);
    int32_t RewardRecord = m_pPlayer->getRecord(1014);
    
    for (int32_t i = 0; i < LoginCount && i <= 6; ++i)
    {
        if (((RewardRecord >> i) & 1) == 0)
        {
            return true;
        }
    }
    
    return false;
}

void DailyActivity::OnGetSignDailyReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return;
    
    int8_t Count = inPacket->readInt8();
    
    tm tmNow;
    m_pPlayer->getLocalNow(&tmNow);
    
    int32_t signCount = 0;
    for (int i = 0; i < tmNow.tm_mday; ++i)
    {
        signCount += (m_signInfo.sign_record >> i) & 1;
    }
    
    if (signCount < Count)
        return;
    
    for (int8_t record : m_signInfo.sign_reward)
    {
        if (record == Count)
            return;
    }
    
    CfgData* cfgData = CfgData::GetInstance();
    MemChrBagVector* items = cfgData->GetSignReward(Count);
    
    if (!items || items->empty())
        return;
    
    CExtCharBag* Bag = m_pPlayer->GetBag();
    if (!Bag->AddItem(items, ITEM_CHANGE_REASON::ICR_SIGN_REWARD))
        return;
    
    m_signInfo.sign_reward.push_back(Count);
    SendSignInfo();
    SendJiangLiDaTingIcon();
}

void DailyActivity::OnGetSignReward(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    
    int32_t count = inPacket->readInt32();
    
    tm tmNow;
    m_pPlayer->getLocalNow(&tmNow);
    
    int32_t signCount = 0;
    for (int i = 0; i <= 30; ++i)
    {
        signCount += (m_signInfo.sign_record >> i) & 1;
    }
    
    if (signCount < count) return;
    
    for (int8_t record : m_signInfo.sign_reward)
    {
        if (record == count) return;
    }
    
    CfgData* cfgData = CfgData::GetInstance();
    std::vector<MemChrBag> items;
    cfgData->GetSignRewardItems(items, static_cast<int8_t>(count));
    
    if (items.empty()) return;
    
    CExtCharBag* Bag = m_pPlayer->GetBag();
    if (!Bag->AddItem(&items, ITEM_CHANGE_REASON::ICR_SIGN_REWARD)) return;
    
    m_signInfo.sign_reward.push_back(static_cast<int8_t>(count));
    SendSignInfo();
    SendJiangLiDaTingIcon();
}

void DailyActivity::OnQuerySignInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    SendSignInfo();
}

void DailyActivity::OnSign(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return;
    
    int8_t nDay = inPacket->readInt8();
    
    tm tmNow;
    m_pPlayer->getLocalNow(&tmNow);
    
    if (nDay > 0)
    {
        int32_t nSignRecord = (1 << (tmNow.tm_mday - 1)) | m_signInfo.sign_record;
        if (m_signInfo.sign_record == nSignRecord) return;
        
        m_signInfo.sign_record = nSignRecord;
        
        CHuoYueDu* PlayerHuoYueDu = m_pPlayer->GetPlayerHuoYueDu();
        PlayerHuoYueDu->AddHuoYueDuRecord(5, 0, 0);
    }
    else
    {
        // 补签逻辑
        CVip* PlayerVip = m_pPlayer->GetPlayerVip();
        int32_t VipLevel = PlayerVip->GetVipLevel();
        
        CfgData* cfgData = CfgData::GetInstance();
        const VipCfg* pVipCfg = cfgData->GetVipCfg(static_cast<int8_t>(VipLevel));
        
        if (!pVipCfg) return;
        
        int32_t Retroactive = m_pPlayer->GetOperateLimit()->GetLimitCount(37207);
        int32_t VipRetroactive = pVipCfg->Retroactive;
        bool IsVip = Retroactive < VipRetroactive;
        
        for (int i = tmNow.tm_mday - 1; i > 0; --i)
        {
            if (!IsVip && m_pPlayer->GetCurrency(CURRENCY_TYPE::CURRENCY_GOLD) < 100) break;
            
            if (((1 << (i - 1)) & m_signInfo.sign_record) == 0)
            {
                m_signInfo.sign_record |= 1 << (i - 1);
                
                if (IsVip)
                {
                    m_pPlayer->GetOperateLimit()->AddLimitCount(37207, 1);
                }
                else
                {
                    m_pPlayer->DecCurrency(CURRENCY_TYPE::CURRENCY_GOLD, 100,
                                           CURRENCY_CHANGE_REASON::GCR_RETROACTIVE, 0);
                    if (!IsVip) break;
                }
            }
        }
    }
    
    m_signInfo.refresh_time = m_pPlayer->getNow();
    SendSignInfo();
    SendJiangLiDaTingIcon();
}

void DailyActivity::RefreshSearchBack(int32_t nDiffDays)
{
    if (nDiffDays <= 0) return;
    
    std::list<SearchBackRecord> lstToday;
    GetTodaySearchBackRecord(&lstToday, 1);
    
    std::list<SearchBackRecord> lstFree;
    GetTodaySearchBackRecord(&lstFree, 0);
    
    for (int i = 2; i >= 0; --i)
    {
        if (i < nDiffDays)
        {
            if (nDiffDays - 1 == i)
            {
                m_vSeachBackRecord[i] = lstToday;
            }
            else
            {
                m_vSeachBackRecord[i] = lstFree;
            }
        }
        else
        {
            m_vSeachBackRecord[i] = m_vSeachBackRecord[i - nDiffDays];
        }
    }
    
    m_lstTodayInfo.clear();
}

void DailyActivity::RefreshSignInfo()
{
    if (!m_pPlayer) return;
    
    if (DayTime::monthdiff(m_signInfo.refresh_time))
    {
        m_pPlayer->GetOperateLimit()->UpdateLimitCount(37207, 0);
        m_signInfo.sign_record = 0;
        m_signInfo.sign_reward.clear();
    }
    
    SendSignInfo();
}

void DailyActivity::RefreshWeekTime()
{
    if (!m_pPlayer) return;
    
    int32_t Record = m_pPlayer->getRecord(1018);
    int32_t DiffWeek = DayTime::weekdiff(Record);
    
    if (DiffWeek == 1)
    {
        int32_t v2 = m_pPlayer->getRecord(1016);
        m_pPlayer->updateRecord(1017, v2);
        m_pPlayer->updateRecord(1016, 0);
        
        CVplan* Vplan = m_pPlayer->GetVplan();
        Vplan->RefreshWeekTime(1);
    }
    else if (DiffWeek > 1)
    {
        m_pPlayer->updateRecord(1017, 0);
        m_pPlayer->updateRecord(1016, 0);
        
        CVplan* Vplan = m_pPlayer->GetVplan();
        Vplan->RefreshWeekTime(DiffWeek);
    }
    
    if (DiffWeek > 0)
    {
        m_pPlayer->updateRecord(37212, 0);
        m_pPlayer->updateRecord(37213, 0);
        
        CExtCharFamily* CharFamily = m_pPlayer->GetCharFamily();
        CharFamily->RefreshWeekTime();
    }
    
    ChouJiang* PlayerChouJiang = m_pPlayer->GetPlayerChouJiang();
    PlayerChouJiang->RefreshWeekTime(DiffWeek);
    
    int32_t Now = m_pPlayer->getNow();
    m_pPlayer->updateRecord(1018, Now);
    
    SendOnlineRewardInfo();
}

void DailyActivity::SendSignInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = m_pPlayer->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, PackType::PACK_DISPATCH, 0x27F9);
    
    if (packet)
    {
        packet->writeInt32(m_signInfo.sign_record);
        packet->writeInt8(static_cast<int8_t>(m_signInfo.sign_reward.size()));
        
        for (int8_t record : m_signInfo.sign_reward)
        {
            packet->writeInt8(record);
        }
        
        packet->writeInt8(static_cast<int8_t>(m_pPlayer->getRecord(37207)));
        packet->setSize(packet->getWOffset());
        
        gameService->sendPacketTo(m_pPlayer->getConnId(), m_pPlayer->getGateIndex(), packet);
    }
}

void DailyActivity::UpdateSearchBackRecord(int32_t nId, int32_t Times)
{
    for (int i = 2; i >= 0; --i)
    {
        for (auto it = m_vSeachBackRecord[i].begin(); it != m_vSeachBackRecord[i].end(); ++it)
        {
            if (it->nId == nId)
            {
                CfgData* cfgData = CfgData::GetInstance();
                const CfgSearchBack* pCfgSearchBack = cfgData->GetSearchBack(nId);
                
                if (pCfgSearchBack)
                {
                    if (pCfgSearchBack->nType == 2 && pCfgSearchBack->nSubType == 16)
                    {
                        m_vSeachBackRecord[i].erase(it);
                    }
                    else
                    {
                        it->nTimes -= Times;
                        if (it->nTimes <= 0)
                        {
                            m_vSeachBackRecord[i].erase(it);
                        }
                    }
                }
                return;
            }
        }
    }
}

void DailyActivity::SetPlayer(Player* player)
{
    m_pPlayer = player;
}

void DailyActivity::GetHuoDongDaTingIcon(ShowIcon* icon)
{
    // 获取活动大厅图标状态
    if (icon)
    {
        icon->nId = 1;
        icon->nState = 2;
        icon->nLeftTime = 0;
    }
}

void DailyActivity::GetJiangLiDaTingIcon(ShowIcon* icon)
{
    // 获取奖励大厅图标状态
    if (icon)
    {
        icon->nId = 2;
        icon->nState = HaveOnLineReward() ? 2 : 0;
        icon->nLeftTime = 0;
    }
}

void DailyActivity::GetSearchBackIcon(ShowIcon* icon)
{
    // 获取找回图标状态
    if (icon)
    {
        icon->nId = 3;
        icon->nState = HaveSearchBackReward() ? 2 : 0;
        icon->nLeftTime = 0;
    }
}

void DailyActivity::GetSevenDayLoginIcon(ShowIcon* icon)
{
    // 获取七日登录图标状态
    if (icon)
    {
        icon->nId = 4;
        icon->nState = HaveSevenLoginReward() ? 2 : 0;
        icon->nLeftTime = 0;
    }
}

SearchBackRecord DailyActivity::GetSearchBackRecord(int32_t nId)
{
    SearchBackRecord result;
    memset(&result, 0, sizeof(result));
    result.nId = nId;
    return result;
}

void DailyActivity::GetTodaySearchBackRecord(std::list<SearchBackRecord>* lst, int32_t type)
{
    // 获取今日找回记录
    lst->clear();
}

std::string DailyActivity::GetOnlineRewardString()
{
    std::string result;
    for (const auto& pair : m_OnlimeTimeGetReward)
    {
        if (!result.empty()) result += "|";
        result += std::to_string(pair.first) + ":|" + std::to_string(pair.second);
    }
    return result;
}