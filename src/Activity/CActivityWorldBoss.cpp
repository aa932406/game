#include "Activity/CActivityWorldBoss.h"
#include "Activity/CActivityMap.h"
#include "Activity/MonsterActivity.h"
#include "Config/CfgActivity.h"
#include "Config/CfgData.h"
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Map/Map.h"
#include "Game/CTimer.h"
#include "Database/DBService.h"
#include "Network/NetPacket.h"
#include "Other/Logger.h"
#include "Character/CExtCharBag.h"
#include "Other/DayTime.h"
#include <algorithm>

CActivityWorldBoss::CActivityWorldBoss(const CfgActivity *const cfgActivity)
    : CActivity(cfgActivity)
    , m_IsBossDie(0)
{
}

CActivityWorldBoss::~CActivityWorldBoss()
{
    m_ReviveTimes.clear();
    m_sScoreRank.clear();
    m_mPlayerScore.clear();
}

int32_t CActivityWorldBoss::GetRevive(Player* player)
{
    return CActivity::GetRevive(player);
}

bool CActivityWorldBoss::OnSitRevive(Player* player)
{
    return player != nullptr;
}

void CActivityWorldBoss::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
}

void CActivityWorldBoss::AddPlayer(Player* player)
{
    if (!player) return;
    
    int64_t cid = player->getCid();
    auto it = m_mPlayerScore.find(cid);
    
    if (it != m_mPlayerScore.end())
    {
        it->second.bInActivity = 1;
        it->second.nStartTick = player->getTick();
    }
    else
    {
        WorldBossPlayerScore score;
        score.nCharId = player->getCid();
        score.strName = player->getName();
        score.bInActivity = 1;
        score.nStartTick = player->getTick();
        score.nDamage = 0;
        score.nIndex = 0;
        score.DamageTick = 0;
        score.connId = player->getConnId();
        
        m_mPlayerScore[cid] = score;
        m_ReviveTimes[cid] = 0;
    }
    
    CActivity::AddPlayer(player);
}

void CActivityWorldBoss::AddPlayerScore(Player* player, int64_t damage)
{
    if (m_nState == ACTIVITY_STATE::AS_END || !player) return;
    
    int64_t Score = 0;
    if (damage <= 100000)
    {
        Score = 1;
    }
    else
    {
        Score = damage / 100000;
    }
    
    int64_t cid = player->getCid();
    auto it = m_mPlayerScore.find(cid);
    
    if (it != m_mPlayerScore.end())
    {
        it->second.nDamage += static_cast<int32_t>(Score);
        it->second.DamageTick = player->getTick();
        SetNeedBroadcastActivityScore(true);
        RefreshRank();
    }
}

void CActivityWorldBoss::AddRewards()
{
    RefreshRank();
    
    for (auto& pair : m_mPlayerScore)
    {
        WorldBossPlayerScore* score = &pair.second;
        
        int32_t nGiftId = 0;
        int32_t nMailId = 0;
        
        if (score->nIndex <= 10 || score->bInActivity)
        {
            if (score->nDamage > 0)
            {
                if (score->nIndex == 1)
                {
                    if (m_cfgActivity->gift_id.size() > 1)
                        nGiftId = m_cfgActivity->gift_id[1];
                    nMailId = 6502;
                }
                else if (score->nIndex <= 3)
                {
                    if (m_cfgActivity->gift_id.size() > 2)
                        nGiftId = m_cfgActivity->gift_id[2];
                    nMailId = 6503;
                }
                else if (score->nIndex <= 10)
                {
                    if (m_cfgActivity->gift_id.size() > 3)
                        nGiftId = m_cfgActivity->gift_id[3];
                    nMailId = 6504;
                }
                else
                {
                    if (m_cfgActivity->gift_id.size() > 4)
                        nGiftId = m_cfgActivity->gift_id[4];
                    nMailId = 6501;
                }
            }
            else
            {
                if (!m_cfgActivity->gift_id.empty())
                    nGiftId = m_cfgActivity->gift_id[0];
                nMailId = 6501;
            }
            
            if (nGiftId > 0)
            {
                MemChrBag bagItem;
                memset(&bagItem, 0, sizeof(bagItem));
                bagItem.itemClass = 1;
                bagItem.itemId = nGiftId;
                bagItem.itemCount = 1;
                
                DBService* dbService = DBService::GetInstance();
                dbService->OnSendSysMail(score->connId, score->nCharId, nMailId, 
                                         &bagItem, ITEM_CHANGE_REASON::ICR_WORLD_BOSS_KILL_REWARD,
                                         nullptr, 0);
            }
        }
    }
}

void CActivityWorldBoss::BroadcastActivityResult()
{
    for (auto pMap : m_activityMaps)
    {
        if (!pMap) continue;
        
        GameService* gameService = GameService::GetInstance();
        Answer::NetPacket* packet = gameService->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2E27);
        
        if (packet)
        {
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt8(m_IsBossDie);
            packet->setSize(packet->getWOffset());
            pMap->broadcast(packet);
        }
    }
}

void CActivityWorldBoss::BroadcastBossKilled(const std::string& name, int64_t cid)
{
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CD6);
    
    if (packet)
    {
        packet->writeInt32(324);
        packet->writeUTF8(name);
        packet->writeInt64(cid);
        packet->setSize(packet->getWOffset());
        gameService->worldBroadcast(packet);
    }
}

void CActivityWorldBoss::BroadcastEnd()
{
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CD6);
    
    if (packet)
    {
        packet->writeInt32(325);
        packet->writeInt32(GetId());
        packet->setSize(packet->getWOffset());
        gameService->worldBroadcast(packet);
    }
}

void CActivityWorldBoss::BroadcastStart()
{
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CD6);
    
    if (packet)
    {
        packet->writeInt32(323);
        packet->writeInt32(GetId());
        packet->setSize(packet->getWOffset());
        gameService->worldBroadcast(packet);
    }
}

bool CActivityWorldBoss::CanEnter(Player* player, CActivityMap* pTargetMap)
{
    return CActivity::CanEnter(player, pTargetMap);
}

int32_t CActivityWorldBoss::GetAutoReviveTime(Player* player)
{
    if (!player) return 0;
    
    int64_t CharId = player->getCid();
    int32_t Time = 0;
    
    auto it = m_ReviveTimes.find(CharId);
    if (it != m_ReviveTimes.end())
    {
        Time = it->second;
    }
    
    int32_t reviveTime = 5 * Time;
    if (reviveTime <= 30)
    {
        return 5000 * Time;
    }
    
    return 30000;
}

bool CActivityWorldBoss::NeedBroadcastActivityScore(CActivityMap* pMap)
{
    if (!pMap) return false;
    
    if (m_nBraodcastActivityScoreSign <= 0) return false;
    
    if (pMap->getTick() - m_nBroadcastActivityScoreTick <= 2999) return false;
    
    m_nBroadcastActivityScoreTick = pMap->getTick();
    --m_nBraodcastActivityScoreSign;
    return true;
}

void CActivityWorldBoss::OnMonsterAdd(MonsterActivity* pMonster)
{
    if (!pMonster) return;
    
    if (pMonster->IsBoss())
    {
        pMonster->SetSyncTime(2000);
        SetNeedBroadcastActivityScore(true);
    }
}

void CActivityWorldBoss::OnMonsterDamaged(MonsterActivity* pMonster, uint32_t damage, Player* pAttacker)
{
    if (!pMonster || !pAttacker) return;
    
    if (pMonster->GetBossSign() == 4)
    {
        AddPlayerScore(pAttacker, damage);
        SendPlayerActivityScore(pAttacker);
    }
}

void CActivityWorldBoss::OnMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (!pMonster || !pKiller || !IsRuning()) return;
    
    if (pMonster->GetBossSign() == 4)
    {
        m_IsBossDie = 1;
        
        WorldBossPlayerScore* FirstPlayer = GetRankFirst();
        if (FirstPlayer)
        {
            BroadcastBossKilled(FirstPlayer->strName, FirstPlayer->nCharId);
            
            MemChrBag bagItem;
            memset(&bagItem, 0, sizeof(bagItem));
            
            if (m_cfgActivity->gift_id.size() > 5)
                bagItem.itemId = m_cfgActivity->gift_id[5];
            bagItem.itemClass = 1;
            bagItem.itemCount = 1;
            bagItem.bind = 1;
            
            int32_t nMailId = 6505;
            
            DBService* dbService = DBService::GetInstance();
            dbService->OnSendSysMail(FirstPlayer->connId, FirstPlayer->nCharId, nMailId,
                                     &bagItem, ITEM_CHANGE_REASON::ICR_WORLD_BOSS_KILL_REWARD,
                                     nullptr, 0);
        }
    }
}

void CActivityWorldBoss::OnPlayerKilled(Player* pDier, Player* pKiller)
{
    if (!pDier || m_nState == ACTIVITY_STATE::AS_END) return;
    
    int64_t cid = pDier->getCid();
    auto itRevive = m_ReviveTimes.find(cid);
    if (itRevive != m_ReviveTimes.end())
    {
        itRevive->second++;
    }
    else
    {
        m_ReviveTimes[cid] = 1;
    }
    
    if (pKiller)
    {
        int32_t Score = 0;
        auto itScore = m_mPlayerScore.find(cid);
        if (itScore != m_mPlayerScore.end())
        {
            Score = itScore->second.nDamage / 100;
            if (Score > 20) Score = 20;
            
            if (Score > 0)
            {
                itScore->second.nDamage -= Score;
            }
        }
        
        if (Score > 0)
        {
            int64_t Cid = pKiller->getCid();
            auto itKiller = m_mPlayerScore.find(Cid);
            if (itKiller != m_mPlayerScore.end())
            {
                itKiller->second.nDamage += Score;
            }
        }
        
        RefreshRank();
        SetNeedBroadcastActivityScore(true);
        SendPlayerActivityScore(pDier);
        SendPlayerActivityScore(pKiller);
    }
    else
    {
        SendPlayerActivityScore(pDier);
    }
}

void CActivityWorldBoss::OnTimeEnd()
{
    AddRewards();
    BroadcastEnd();
    StopActivity();
    m_nState = ACTIVITY_STATE::AS_END;
    DelayKickAll(30);
}

void CActivityWorldBoss::RefreshRank()
{
    m_sScoreRank.clear();
    
    for (auto& pair : m_mPlayerScore)
    {
        if (pair.second.nDamage > 0)
        {
            m_sScoreRank.insert(&pair.second);
        }
    }
    
    int32_t nIndex = 1;
    for (auto scorePtr : m_sScoreRank)
    {
        if (!scorePtr) continue;
        
        if (scorePtr->nIndex != nIndex)
        {
            scorePtr->nIndex = nIndex;
            
            if (scorePtr->bInActivity)
            {
                GameService* gameService = GameService::GetInstance();
                Player* pPlayer = gameService->getPlayer(scorePtr->nCharId, 0, 0);
                if (pPlayer)
                {
                    SendPlayerActivityScore(pPlayer);
                }
            }
        }
        ++nIndex;
    }
}

void CActivityWorldBoss::RemovePlayer(Player* player, bool islogout)
{
    CActivity::RemovePlayer(player, islogout);
    
    if (!player) return;
    
    int64_t cid = player->getCid();
    auto it = m_mPlayerScore.find(cid);
    if (it != m_mPlayerScore.end())
    {
        it->second.bInActivity = 0;
    }
}

void CActivityWorldBoss::Reset()
{
    CActivity::Reset();
    m_mPlayerScore.clear();
    m_sScoreRank.clear();
    m_ReviveTimes.clear();
    m_IsBossDie = 0;
}

void CActivityWorldBoss::SendPlayerScore(Player* player)
{
    if (!player) return;
    
    int32_t DieTimes = 0;
    int64_t cid = player->getCid();
    
    auto itRevive = m_ReviveTimes.find(cid);
    if (itRevive != m_ReviveTimes.end())
    {
        DieTimes = itRevive->second;
    }
    
    auto itScore = m_mPlayerScore.find(cid);
    if (itScore == m_mPlayerScore.end()) return;
    
    int8_t ConnId = player->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2E24);
    
    if (!packet) return;
    
    WorldBossPlayerScore* score = &itScore->second;
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt32(score->nIndex);
    packet->writeInt32(score->nDamage);
    packet->writeInt32(DieTimes);
    packet->setSize(packet->getWOffset());
    
    gameService->sendPacketTo(player->getConnId(), player->getGateIndex(), packet);
}

WorldBossPlayerScore* CActivityWorldBoss::GetRankFirst()
{
    if (m_sScoreRank.empty()) return nullptr;
    return *m_sScoreRank.begin();
}

bool WorldBossPlayerScoreRankGreater::operator()(const WorldBossPlayerScore* lhs, const WorldBossPlayerScore* rhs) const
{
    if (!lhs || !rhs) return false;
    
    if (lhs->nDamage != rhs->nDamage)
    {
        return lhs->nDamage > rhs->nDamage;
    }
    
    return lhs->DamageTick < rhs->DamageTick;
}