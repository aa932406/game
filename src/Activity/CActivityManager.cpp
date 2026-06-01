#include "Activity/CActivityManager.h"
#include "Activity/CActivity.h"
#include "Activity/CFamilyWar.h"
#include "Activity/CFamilyLight.h"
#include "Activity/CMoLingRuQin.h"
#include "Activity/CActivityWorldBoss.h"
#include "Activity/CCampWar.h"
#include "Activity/CPeerlessWar.h"
#include "Activity/CCityWar.h"
#include "Activity/UltimateChallenge.h"
#include "Activity/CYaBiao.h"
#include "Activity/CKaiFuBoss.h"
#include "Activity/CXinMaiMap.h"
#include "Activity/CrossTower.h"
#include "Activity/CRongLian.h"
#include "Activity/CRefreshMonster.h"
#include "Activity/CTianJiangBaoXiang.h"
#include "Activity/CSqiderQueen.h"
#include "Config/CfgData.h"
#include "Database/DBService.h"
#include "Other/Logger.h"
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Map/MapManager.h"
#include "Map/Map.h"
#include "Game/CTimer.h"
#include "Other/DayTime.h"
#include "Other/GameMsg.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharFamily.h"

CActivityManager::CActivityManager()
    : m_CrossTowerWiner{0, 0}
{
    m_mWinFamily.clear();
    m_mPeerlessWinner.clear();
    m_mActivity.clear();
}

CActivityManager::~CActivityManager()
{
    for (auto &pair : m_mActivity)
    {
        if (pair.second)
        {
            delete pair.second;
            pair.second = nullptr;
        }
    }
    m_mActivity.clear();
    m_mPeerlessWinner.clear();
    m_mWinFamily.clear();
}

void CActivityManager::AddTianJiangBaoXiangCount(Player* player, int32_t AddCount)
{
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (!pActivity) continue;
        
        CTianJiangBaoXiang* pBaoXiang = dynamic_cast<CTianJiangBaoXiang*>(pActivity);
        if (pBaoXiang && static_cast<int32_t>(pBaoXiang->GetState()) == 2)
        {
            pBaoXiang->AddOpenCount(player, AddCount);
            return;
        }
    }
}

void CActivityManager::AppendActivityState(Answer::NetPacket* packet, int32_t* nCount)
{
    if (!packet || !nCount) return;
    
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (!pActivity) continue;
        
        int32_t nState = static_cast<int32_t>(pActivity->GetState());
        
        if (nState == 0)
        {
            nState = 1;
        }
        else if (nState == 4)
        {
            nState = 5;
        }
        
        if (nState != 0)
        {
            int8_t Id = pActivity->GetId();
            packet->writeInt8(Id);
            packet->writeInt8(nState);
            ++(*nCount);
        }
    }
}

void CActivityManager::ApplyCityWar(Player* player, int32_t cityId, int64_t FamilyId, int32_t Type)
{
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (!pActivity) continue;
        
        CCityWar* pCityWar = dynamic_cast<CCityWar*>(pActivity);
        if (!pCityWar) continue;
        
        int32_t NextStartTime = pCityWar->GetNextStartTime();
        if (NextStartTime <= 0) continue;
        
        if (NextStartTime - DayTime::now() > 7200)
        {
            pCityWar->ApplyCityWar(player, FamilyId, static_cast<int8_t>(Type));
            return;
        }
    }
}

void CActivityManager::GetActicityIconState(Player* player, std::list<ShowIcon>& iconList)
{
    if (!player) return;
    
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (pActivity)
        {
            pActivity->GetIconState(iconList);
        }
    }
}

int32_t CActivityManager::GetCrossTowerName()
{
    return m_CrossTowerWiner.Winner;
}

int32_t CActivityManager::GetCurActivityId(int32_t nType)
{
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (!pActivity) continue;
        
        if (pActivity->GetType() == nType && pActivity->IsRuning() && pActivity->IsRightLine())
        {
            return pActivity->GetId();
        }
    }
    return 0;
}

void CActivityManager::GiveDailyReward(Player* player, int32_t nId)
{
    auto it = m_mActivity.find(nId);
    if (it != m_mActivity.end() && it->second)
    {
        it->second->GiveDailyReward(player);
    }
}

int32_t CActivityManager::HaveRewardCount(Player* player, int32_t nId)
{
    auto it = m_mActivity.find(nId);
    if (it != m_mActivity.end() && it->second)
    {
        return it->second->HaveRewardCount(player);
    }
    return 0;
}

void CActivityManager::Init(int32_t line)
{
    if (line == 9)
    {
        DBService* dbService = DBService::GetInstance();
        dbService->onLoadCityWarInfo();
        dbService->onLoadPeerlessWarInfo();
    }
    
    CfgData* cfgData = CfgData::GetInstance();
    const std::map<int, CfgActivity>* cfgAllActivity = cfgData->GetAllActivity();
    
    if (!cfgAllActivity) return;
    
    for (const auto& pair : *cfgAllActivity)
    {
        const CfgActivity& cfgActivity = pair.second;
        CActivity* pActivity = nullptr;
        
        switch (cfgActivity.typeId)
        {
        case 1:  // 家族战
            pActivity = new CFamilyWar(&cfgActivity);
            break;
            
        case 3:  // 家族之光
            pActivity = new CFamilyLight(&cfgActivity);
            break;
            
        case 4:  // 魔灵入侵
            pActivity = new CMoLingRuQin(&cfgActivity);
            break;
            
        case 11: // 世界BOSS
            pActivity = new CActivityWorldBoss(&cfgActivity);
            break;
            
        case 12: // 阵营战
            pActivity = new CCampWar(&cfgActivity);
            break;
            
        case 13: // 无双战
            pActivity = new CPeerlessWar(&cfgActivity);
            break;
            
        case 18: // 城战
            pActivity = new CCityWar(&cfgActivity);
            break;
            
        case 19: // 终极挑战
            pActivity = new UltimateChallenge(&cfgActivity);
            break;
            
        case 20: // 押镖
            pActivity = new CYaBiao(&cfgActivity);
            break;
            
        case 21: // 开服BOSS
            pActivity = new CKaiFuBoss(&cfgActivity);
            break;
            
        case 22: // 心脉地图
            pActivity = new CXinMaiMap(&cfgActivity);
            break;
            
        case 23: // 跨服塔
            pActivity = new CrossTower(&cfgActivity);
            break;
            
        case 24: // 融炼
            pActivity = new CRongLian(&cfgActivity);
            break;
            
        case 26: // 刷新怪物
            pActivity = new CRefreshMonster(&cfgActivity);
            break;
            
        default:
            LOG_ERROR("CActivityManager::Init() actid=%d unknown typeId=%d", 
                      cfgActivity.id, cfgActivity.typeId);
            break;
        }
        
        if (pActivity)
        {
            pActivity->Init();
            m_mActivity[cfgActivity.id] = pActivity;
        }
        else
        {
            LOG_ERROR("CActivityManager::Init() actid=%d create Activity fail!!!", cfgActivity.id);
        }
    }
}

bool CActivityManager::IsActivityRunning(int32_t activityId)
{
    auto it = m_mActivity.find(activityId);
    if (it != m_mActivity.end() && it->second)
    {
        return it->second->IsRuning();
    }
    return false;
}

bool CActivityManager::IsActivityRunningByType(int32_t typeId)
{
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (pActivity && pActivity->GetType() == typeId && pActivity->IsRuning())
        {
            return true;
        }
    }
    return false;
}

void CActivityManager::NotifyActivityInfo(Player* player)
{
    if (!player) return;
    
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (pActivity)
        {
            pActivity->SendPlayerActivityInfo(player);
        }
    }
}

void CActivityManager::OnCityWarResult(int32_t nActId, int8_t connid, int64_t nFamilyId,
                                        int32_t nWinTime, int32_t nTime, int64_t nLeaderCid,
                                        int32_t First, int32_t Second, int32_t Third, int32_t nIndex,
                                        const std::string& p_FirstFamilyName,
                                        const std::string& p_SecondFamilyName,
                                        const std::string& p_ThirdFamilyName)
{
    auto itWin = m_mWinFamily.find(connid);
    if (itWin != m_mWinFamily.end() && itWin->second != nFamilyId)
    {
        int64_t oldFamilyId = itWin->second;
        GameService* gameService = GameService::GetInstance();
        gameService->UpdateCityWarTitle(oldFamilyId, nFamilyId);
        itWin->second = nFamilyId;
        
        Answer::NetPacket* packet = gameService->popNetpacket(connid, Answer::PackType::PACK_DISPATCH, 0x2CC5);
        if (packet)
        {
            packet->writeInt64(nFamilyId);
            packet->setSize(packet->getWOffset());
            gameService->worldBroadcast(connid, packet);
        }
    }
    
    auto it = m_mActivity.find(nActId);
    if (it != m_mActivity.end())
    {
        CCityWar* pCityWar = dynamic_cast<CCityWar*>(it->second);
        if (pCityWar)
        {
            pCityWar->OnCityWarResult(nFamilyId, nWinTime, nTime, nLeaderCid,
                                       First, Second, Third, nIndex,
                                       p_FirstFamilyName, p_SecondFamilyName, p_ThirdFamilyName);
        }
    }
}

void CActivityManager::OnCrossTower(int32_t result, int64_t Winner, int32_t Battle, const std::string& p_Name, int8_t connid)
{
    if (m_CrossTowerWiner.Winner != Winner)
    {
        if (m_CrossTowerWiner.Winner > 0)
        {
            GameService* gameService = GameService::GetInstance();
            gameService->onRemoveTitle(m_CrossTowerWiner.Winner, 9);
        }
        
        m_CrossTowerWiner.Winner = Winner;
        m_CrossTowerWiner.Battle = Battle;
        m_CrossTowerWiner.Name = p_Name;
        
        if (m_CrossTowerWiner.Winner > 0)
        {
            GameService* gameService = GameService::GetInstance();
            gameService->onCheckTitle(m_CrossTowerWiner.Winner, 9, 1);
        }
        
        GameService* gameService = GameService::GetInstance();
        if (gameService->getLine() != 9)
        {
            Answer::NetPacket* packet = gameService->popNetpacket(connid, Answer::PackType::PACK_DISPATCH, 0x2CC7);
            if (packet)
            {
                packet->writeUTF8(m_CrossTowerWiner.Name);
                packet->writeInt64(m_CrossTowerWiner.Winner);
                packet->setSize(packet->getWOffset());
                gameService->worldBroadcast(connid, packet);
            }
        }
        
        gameService->RecalAllPlayerAttr();
    }
}

void CActivityManager::OnPeerlessWarResult(int32_t nActId, int8_t connid, int64_t winner, const std::string& name)
{
    auto itWin = m_mPeerlessWinner.find(connid);
    if (itWin != m_mPeerlessWinner.end() && itWin->second != winner)
    {
        if (itWin->second > 0)
        {
            GameService* gameService = GameService::GetInstance();
            gameService->onRemoveTitle(itWin->second, 2);
        }
        itWin->second = winner;
        
        if (winner > 0)
        {
            GameService* gameService = GameService::GetInstance();
            gameService->onCheckTitle(winner, 2, 1);
        }
    }
    
    auto it = m_mActivity.find(nActId);
    if (it != m_mActivity.end())
    {
        CPeerlessWar* pAct = dynamic_cast<CPeerlessWar*>(it->second);
        if (pAct)
        {
            pAct->OnPeerlessWarResult(winner, name);
        }
    }
}

void CActivityManager::OnUpdate(int64_t curTick)
{
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (!pActivity) continue;
        
        pActivity->CheckActivity();
        
        if (pActivity->IsRuning())
        {
            pActivity->OnUpdateGlobal(curTick);
        }
    }
}

void CActivityManager::SendActivityInfo(Player* player, int32_t nId)
{
    if (!player) return;
    
    auto it = m_mActivity.find(nId);
    if (it != m_mActivity.end() && it->second)
    {
        it->second->SendPlayerActivityInfo(player);
    }
}

void CActivityManager::SendActivityRankInfo(Player* player, int32_t nId)
{
    if (!player) return;
    
    auto it = m_mActivity.find(nId);
    if (it != m_mActivity.end() && it->second)
    {
        it->second->SendActivityRankInfo(player);
    }
}

void CActivityManager::SetCityWarWinner(int8_t connid, int64_t nFamilyId)
{
    m_mWinFamily[connid] = nFamilyId;
    
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(connid, Answer::PackType::PACK_DISPATCH, 0x2CC5);
    
    if (packet)
    {
        packet->writeInt64(nFamilyId);
        packet->setSize(packet->getWOffset());
        gameService->worldBroadcast(connid, packet);
    }
}

void CActivityManager::SetCrossTowerResult(int64_t nCharId, int32_t Battle, const std::string& p_Name)
{
    m_CrossTowerWiner.Winner = nCharId;
    m_CrossTowerWiner.Battle = Battle;
    m_CrossTowerWiner.Name = p_Name;
}

void CActivityManager::SetCrossTowerWinner(int64_t nCharId, int32_t Battle, const std::string& p_Name)
{
    if (m_CrossTowerWiner.Battle <= Battle)
    {
        m_CrossTowerWiner.Winner = nCharId;
        m_CrossTowerWiner.Battle = Battle;
        m_CrossTowerWiner.Name = p_Name;
        
        int32_t activityId = 15;
        auto it = m_mActivity.find(activityId);
        if (it != m_mActivity.end())
        {
            CrossTower* pCrossTower = dynamic_cast<CrossTower*>(it->second);
            if (pCrossTower)
            {
                pCrossTower->SetWinnerInfo(nCharId, p_Name);
            }
        }
    }
}

void CActivityManager::SetPeerlessWarWinner(int8_t connid, int64_t nCharId)
{
    m_mPeerlessWinner[connid] = nCharId;
}

void CActivityManager::StopSqiderQueen()
{
    for (auto& pair : m_mActivity)
    {
        CActivity* pActivity = pair.second;
        if (!pActivity) continue;
        
        CSqiderQueen* pSqiderQueen = dynamic_cast<CSqiderQueen*>(pActivity);
        if (pSqiderQueen && pSqiderQueen->IsRuning())
        {
            pSqiderQueen->StopActivityBySqiderQueenDie();
        }
    }
}

void CActivityManager::UpdateActivityState(int32_t nId, int32_t nState)
{
    CActivity* pActivity = GetActivity(nId);
    if (!pActivity) return;
    
    pActivity->UpdateActivityState(nState);
    
    if (pActivity->GetType() == 11 && nState == 2)
    {
        MapManager* mapMgr = MapManager::GetInstance();
        Map* pMap = mapMgr->GetMap(50106);
        
        if (pMap)
        {
            int32_t LeftTime = pActivity->GetLeftTime();
            int32_t RunnerId = pMap->GetRunnerId();
            mapMgr->PostMsg(RunnerId, GameMsgCode::GMC_ADD_WORLD_BOSS_NPC, pMap, this, LeftTime, 0, 0);
        }
    }
}

CActivity* CActivityManager::GetActivity(int32_t nId)
{
    auto it = m_mActivity.find(nId);
    if (it != m_mActivity.end())
    {
        return it->second;
    }
    return nullptr;
}

int32_t CActivityManager::GetLine()
{
    GameService* gameService = GameService::GetInstance();
    return gameService->getLine();
}