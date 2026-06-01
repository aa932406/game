#include "Activity/CFestivalActivity.h"
#include "Activity/CActivityManager.h"
#include "Config/CfgData.h"
#include "Config/CfgFestivalActivityTable.h"
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Map/MapManager.h"
#include "Map/Map.h"
#include "Game/CTimer.h"
#include "Database/DBService.h"
#include "Character/CExtCharBag.h"
#include "Other/Logger.h"
#include "Other/Answer.h"
#include "Other/DayTime.h"
#include "Utility/StringUtility.h"
#include <cstring>
#include <cstdio>

CFestivalActivity::CFestivalActivity()
    : m_nDay(-1)
    , m_nMinute(0)
    , m_nStartDay(0)
    , m_nEndDay(0)
    , m_nIcon(0)
    , m_PlantId(0)
    , m_PlantCount(0)
    , m_NpcId(0)
    , m_NpcCount(0)
    , m_Times(0)
    , m_PlantTime(0)
    , m_nVersion(0)
{
    memset(m_vOpen, 0, sizeof(m_vOpen));
    memset(m_vStartDay, 0, sizeof(m_vStartDay));
    memset(m_vEndDay, 0, sizeof(m_vEndDay));
}

CFestivalActivity::~CFestivalActivity()
{
    m_ChangeItemCfgVt.clear();
    m_NpcMapPosVt.clear();
    m_StringVtVector.clear();
    m_PlantMapPosVt.clear();
}

int32_t CFestivalActivity::DaTi(Player* player, const std::string& answerId)
{
    if (!player) return -1;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE_2::FAT2_DA_TI)) return -2;
    
    int32_t DaTiTimes = player->getRecord(2120);
    int32_t index = GetActDay(FESTIVAL_ACTIVITY_TYPE_2::FAT2_DA_TI);
    
    if (index < 0 || index >= static_cast<int32_t>(m_StringVtVector.size())) return -3;
    
    std::vector<std::string>* v5 = m_StringVtVector[index];
    if (!v5) return -4;
    
    int32_t DaAnCount = static_cast<int32_t>(v5->size());
    
    if (DaTiTimes < 0 || DaTiTimes >= DaAnCount) return -5;
    
    std::string* v7 = &(*v5)[DaTiTimes];
    
    if (*v7 == answerId)
    {
        player->updateRecord(2120, DaTiTimes + 1);
        return 0;
    }
    
    return -6;
}

int32_t CFestivalActivity::DuiHuan(Player* player, int32_t exchangeId)
{
    if (!player) return -1;
    
    if (exchangeId < 0 || exchangeId >= static_cast<int32_t>(m_ChangeItemCfgVt.size())) return -2;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE_2::FAT2_DUI_HUAN)) return -3;
    
    int32_t Record = player->getRecord(exchangeId + 10101);
    
    ChangeItemCfg* v6 = m_ChangeItemCfgVt[exchangeId];
    if (!v6) return -4;
    
    if (Record >= v6->Times) return -5;
    
    if (v6->nCostGold > 0)
    {
        int64_t Currency = player->GetCurrency(CURRENCY_TYPE::CURRENCY_GOLD);
        if (Currency < v6->nCostGold) return -6;
    }
    
    CExtCharBag* Bag = player->GetBag();
    if (!Bag->AddAndRemoveItem(&v6->GetItems, &v6->lCostItem, ITEM_CHANGE_REASON::IDCR_FESTIVAL_ACT_2))
    {
        return -7;
    }
    
    if (v6->nCostGold > 0)
    {
        player->DecCurrency(CURRENCY_TYPE::CURRENCY_GOLD, v6->nCostGold, 
                            CURRENCY_CHANGE_REASON::GCR_FESTIVAL_ACT_2, 0);
    }
    
    int32_t v10 = player->getRecord(exchangeId + 10101) + 1;
    player->updateRecord(exchangeId + 10101, v10);
    
    return 0;
}

int32_t CFestivalActivity::GetActDay(FESTIVAL_ACTIVITY_TYPE_2 nType)
{
    int32_t index = m_nDay - m_vStartDay[static_cast<int>(nType)];
    
    if (index >= 0 && index <= m_vEndDay[static_cast<int>(nType)] - m_vStartDay[static_cast<int>(nType)] - 1)
    {
        return index;
    }
    
    return -1;
}

int32_t CFestivalActivity::GetDaTiReward(Player* player, int32_t RewardType)
{
    if (!player) return -1;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE_2::FAT2_DA_TI)) return -2;
    
    int32_t DaTiTimes = player->getRecord(2120);
    int32_t index = GetActDay(FESTIVAL_ACTIVITY_TYPE_2::FAT2_DA_TI);
    
    if (index < 0 || index >= static_cast<int32_t>(m_StringVtVector.size())) return -3;
    
    std::vector<std::string>* v4 = m_StringVtVector[index];
    if (!v4) return -4;
    
    int32_t DaAnCount = static_cast<int32_t>(v4->size());
    
    if (DaTiTimes < DaAnCount) return -5;
    
    int32_t record = 0;
    MemChrBag item;
    memset(&item, 0, sizeof(item));
    
    if (RewardType == 1)
    {
        record = 2121;
        item = m_DayGift;
    }
    else
    {
        record = 37401;
        item = m_ActGift;
    }
    
    if (player->getRecord(record) > 0) return -6;
    
    CExtCharBag* Bag = player->GetBag();
    if (!Bag->AddItem(&item, ITEM_CHANGE_REASON::IDCR_DA_TI_ACT)) return -7;
    
    player->updateRecord(record, 1);
    
    return 0;
}

void CFestivalActivity::GetIconState(Player* player, std::list<ShowIcon>& IconList)
{
    if (!player) return;
    
    if (NeedShowIcon())
    {
        ShowIcon stu;
        memset(&stu, 0, sizeof(stu));
        getIconState(&stu, player);
        IconList.push_back(stu);
    }
}

int32_t CFestivalActivity::GetPlantTimes()
{
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE_2::FAT2_COLLECTION)) return 0;
    return m_Times;
}

int32_t CFestivalActivity::GetPlantTimes2()
{
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE_2::FAT2_COLLECTION2)) return 0;
    return m_PlantTime;
}

void CFestivalActivity::Init(int32_t activityId)
{
    InitCfgData();
    
    int32_t nVersion = LoadVersion();
    if (nVersion > 0 && m_nVersion != nVersion || nVersion == 0)
    {
        SaveVersion();
    }
    
    CTimer* timer = CTimer::GetInstance();
    tm nowTime = timer->GetLocalNow();
    m_nMinute = nowTime.tm_min + 60 * nowTime.tm_hour;
}

bool CFestivalActivity::IsInTime()
{
    return m_nDay >= 0 && m_nDay < m_nEndDay;
}

bool CFestivalActivity::IsOpen(FESTIVAL_ACTIVITY_TYPE_2 nType)
{
    return IsInTime() && (m_vOpen[static_cast<int>(nType)] != 0);
}

void CFestivalActivity::OnDaySwitch()
{
    CTimer* timer = CTimer::GetInstance();
    int32_t nowTime = timer->GetNow();
    int32_t dayZero = DayTime::dayzero(nowTime);
    int32_t startDayZero = DayTime::dayzero(m_nStartDay);
    
    m_nDay = (dayZero - startDayZero) / 86400;
    
    if (m_nDay == m_nEndDay)
    {
        HideIcon(m_nIcon);
    }
}

void CFestivalActivity::OnNewMinute(int32_t nMinute)
{
    m_nMinute = nMinute;
    
    int32_t nLeftTime = GetLeftTime();
    
    if (nLeftTime > 0 && m_nDay == 0 && nMinute == 0)
    {
        // 刷新植物
        if (m_PlantId > 0)
        {
            CfgData* cfgData = CfgData::GetInstance();
            const CfgPlant* pCfgPlant = cfgData->GetCfgPlant(m_PlantId);
            
            if (pCfgPlant && !m_PlantMapPosVt.empty())
            {
                std::random_shuffle(m_PlantMapPosVt.begin(), m_PlantMapPosVt.end());
                
                int32_t nFreshCount = m_PlantCount;
                if (static_cast<int32_t>(m_PlantMapPosVt.size()) < nFreshCount)
                {
                    nFreshCount = static_cast<int32_t>(m_PlantMapPosVt.size());
                }
                
                for (int32_t i = 0; i < nFreshCount; ++i)
                {
                    int32_t mapId = m_PlantMapPosVt[i]->m_MapId;
                    MapManager* mapMgr = MapManager::GetInstance();
                    Map* pMap = mapMgr->GetMap(mapId);
                    
                    if (!pMap) continue;
                    
                    Plant* plant = new Plant();
                    if (plant && pMap)
                    {
                        CfgMapPlant stu;
                        memset(&stu, 0, sizeof(stu));
                        stu.mapid = m_PlantMapPosVt[i]->m_MapId;
                        stu.plantid = m_PlantId;
                        stu.x = m_PlantMapPosVt[i]->m_Pos.x;
                        stu.y = m_PlantMapPosVt[i]->m_Pos.y;
                        
                        plant->init(pMap, pCfgPlant, &stu, 0);
                        plant->setLifeTime(nLeftTime);
                        pMap->addPlant(plant);
                    }
                }
            }
        }
        
        // 刷新NPC
        if (m_NpcId > 0)
        {
            CfgData* cfgData = CfgData::GetInstance();
            const CfgNpc* pCfgNpc = cfgData->GetCfgNpc(m_NpcId);
            
            if (pCfgNpc && !m_NpcMapPosVt.empty())
            {
                std::random_shuffle(m_NpcMapPosVt.begin(), m_NpcMapPosVt.end());
                
                int32_t nFreshCount = m_NpcCount;
                if (static_cast<int32_t>(m_NpcMapPosVt.size()) < nFreshCount)
                {
                    nFreshCount = static_cast<int32_t>(m_NpcMapPosVt.size());
                }
                
                for (int32_t i = 0; i < nFreshCount; ++i)
                {
                    int32_t mapId = m_NpcMapPosVt[i]->m_MapId;
                    MapManager* mapMgr = MapManager::GetInstance();
                    Map* pMap = mapMgr->GetMap(mapId);
                    
                    if (!pMap) continue;
                    
                    Npc* npc = new Npc();
                    if (npc && pMap)
                    {
                        int32_t now = DayTime::now();
                        npc->init(pCfgNpc, nLeftTime + now);
                        pMap->addNpc(npc, m_NpcMapPosVt[i]->m_Pos.x, m_NpcMapPosVt[i]->m_Pos.y);
                    }
                }
            }
        }
    }
}

void CFestivalActivity::SendIconState(Player* player)
{
    if (!player) return;
    
    if (NeedShowIcon())
    {
        ShowIcon stu;
        memset(&stu, 0, sizeof(stu));
        getIconState(&stu, player);
        player->SendIconState(&stu);
    }
}

int32_t CFestivalActivity::GetLeftTime()
{
    if (m_nDay < 0) return 0;
    
    CTimer* timer = CTimer::GetInstance();
    int32_t nNowTime = timer->GetNow();
    
    return 86400 * (m_nEndDay - m_nDay) - (nNowTime - DayTime::dayzero(nNowTime));
}

void CFestivalActivity::HideIcon(int32_t nIconId)
{
    GameService* gameService = GameService::GetInstance();
    if (gameService->getLine() != 1) return;
    
    Answer::NetPacket* packet = gameService->popNetpacket(0, PackType::PACK_DISPATCH, 0x2CC3);
    if (!packet) return;
    
    packet->writeInt32(nIconId);
    packet->writeInt8(4);
    packet->writeInt32(0);
    packet->writeInt8(0);
    packet->writeInt32(0);
    packet->writeInt8(0);
    packet->setSize(packet->getWOffset());
    
    gameService->worldBroadcast(0, packet);
}

void CFestivalActivity::InitCfgData()
{
    CfgData* cfgData = CfgData::GetInstance();
    
    for (auto& pair : m_ChangeItemCfgVt)
    {
        if (pair)
            delete pair;
    }
    m_ChangeItemCfgVt.clear();
    
    for (auto& vec : m_StringVtVector)
    {
        if (vec)
            delete vec;
    }
    m_StringVtVector.clear();

    m_PlantMapPosVt.clear();
    m_NpcMapPosVt.clear();
    
    CfgFestivalActivityTable* pTable = cfgData->GetFestivalActivityTable();
    if (!pTable) return;
    
    const std::map<int, CfgFestivalActivity>* pActMap = pTable->GetAll();
    if (!pActMap || pActMap->empty()) return;
    
    const CfgFestivalActivity* pCfg = &pActMap->begin()->second;
    
    m_nIcon = pCfg->nIcon;
    m_nStartDay = pCfg->nStartDay;
    m_nEndDay = pCfg->nEndDay;
    m_PlantId = pCfg->nPlantId;
    m_PlantCount = pCfg->nPlantCount;
    m_NpcId = pCfg->nNpcId;
    m_NpcCount = pCfg->nNpcCount;
    m_DayGift = pCfg->dayGift;
    m_ActGift = pCfg->actGift;
    m_Times = pCfg->nTimes;
    m_PlantTime = pCfg->nPlantTime;
    
    // 转换植物位置（从值类型到指针类型）
    for (const auto& pos : pCfg->vPlantPosList)
    {
        MapPos* mapPos = new MapPos();
        mapPos->m_MapId = pos.nMapId;
        mapPos->m_Pos.x = pos.nX;
        mapPos->m_Pos.y = pos.nY;
        m_PlantMapPosVt.push_back(mapPos);
    }
    
    // 转换NPC位置（从值类型到指针类型）
    for (const auto& pos : pCfg->vNpcPosList)
    {
        MapPos* mapPos = new MapPos();
        mapPos->m_MapId = pos.nMapId;
        mapPos->m_Pos.x = pos.nX;
        mapPos->m_Pos.y = pos.nY;
        m_NpcMapPosVt.push_back(mapPos);
    }
    
    // 转换兑换配置（从值类型到指针类型）
    for (const auto& cfgItem : pCfg->vChangeCfgList)
    {
        ChangeItemCfg* item = new ChangeItemCfg();
        item->Times = cfgItem.nTimes;
        item->nCostGold = cfgItem.nCostGold;
        item->GetItems = cfgItem.vGetItems;
        item->lCostItem = cfgItem.vCostItems;
        m_ChangeItemCfgVt.push_back(item);
    }
    
    memset(m_vStartDay, 0, sizeof(m_vStartDay));
    memset(m_vEndDay, 0, sizeof(m_vEndDay));
    for (size_t i = 0; i < pCfg->vStartDayList.size() && i < 5; ++i)
    {
        m_vStartDay[i] = pCfg->vStartDayList[i];
        m_vEndDay[i] = pCfg->vEndDayList[i];
    }
    
    for (const auto& answerList : pCfg->vAnswerList)
    {
        std::vector<std::string>* vAnswers = new std::vector<std::string>(answerList);
        m_StringVtVector.push_back(vAnswers);
    }
}

void CFestivalActivity::InitOpenList(const std::string& str)
{
    memset(m_vOpen, 0, sizeof(m_vOpen));
    
    std::vector<std::string> vOpen;
    StringUtility::split(vOpen, str, "|");
    
    for (const auto& item : vOpen)
    {
        int32_t nType = atoi(item.c_str());
        if (nType > 0 && nType <= 5)
        {
            m_vOpen[nType] = 1;
        }
    }
}

int32_t CFestivalActivity::LoadVersion()
{
    DBService* dbService = DBService::GetInstance();
    // 从数据库加载版本号
    // 具体实现取决于数据库访问方式
    
    return 0;
}

bool CFestivalActivity::NeedShowIcon()
{
    GameService* gameService = GameService::GetInstance();
    if (gameService->getLine() == 9) return false;
    
    return m_nDay >= 0 && m_nDay < m_nEndDay;
}

void CFestivalActivity::SaveVersion()
{
    DBService* dbService = DBService::GetInstance();
    int32_t nNowTime = DayTime::now();
    
    // 保存版本号到数据库
    // 具体实现取决于数据库访问方式
}

void CFestivalActivity::getIconState(ShowIcon* stu, Player* player)
{
    if (!stu || !player) return;
    
    stu->nId = m_nIcon;
    stu->nState = IsOpen(FESTIVAL_ACTIVITY_TYPE_2::FAT2_MAIN) ? 2 : 0;
    stu->nLeftTime = GetLeftTime();
    
    if (HasReward(player))
    {
        stu->nEffects = 1;
    }
}

bool CFestivalActivity::HasReward(Player* player)
{
    if (!player) return false;
    
    // 检查是否有可领取奖励
    for (size_t i = 0; i < m_ChangeItemCfgVt.size(); ++i)
    {
        int32_t Record = player->getRecord(static_cast<int32_t>(i) + 10101);
        if (Record < m_ChangeItemCfgVt[i]->Times)
        {
            return true;
        }
    }
    
    return false;
}