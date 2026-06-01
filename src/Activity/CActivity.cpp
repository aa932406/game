#include "Activity/CActivity.h"
#include "Activity/CActivityMap.h"
#include "Activity/DailyActivity.h"
#include "Activity/CActivityManager.h"
#include "Config/CfgActivity.h"
#include "Other/Logger.h"
#include "Game/GameService.h"
#include "Game/CTimer.h"
#include "Game/Player.h"
#include "Map/Map.h"
#include "Map/MapManager.h"
#include "Database/DBService.h"
#include "Other/Answer.h"
#include "Network/NetPacket.h"
#include "Other/Buff.h"
#include "Config/CfgData.h"
#include "Other/DayTime.h"
#include "Other/GameMsg.h"
#include <algorithm>

CActivity::CActivity(const CfgActivity *const cfgActivity)
    : m_cfgActivity(cfgActivity)
    , m_nState(ACTIVITY_STATE::AS_NOT_START)
    , m_nStartTime(0)
    , m_nKickTime(0)
    , m_nBraodcastActivityScoreSign(0)
    , m_nBroadcastActivityScoreTick(0)
    , m_nLastReviveCheckTick(0)
{
}

CActivity::~CActivity()
{
    m_players.clear();
    m_activityMaps.clear();
}

Answer::NetPacket* CActivity::CreateScorePacket(Player* player, int32_t score)
{
    if (!player) return nullptr;
    
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(player->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2E23);
    
    if (packet)
    {
        packet->writeInt32(m_cfgActivity->id);
        packet->writeInt32(score);
        packet->setSize(packet->getWOffset());
    }
    
    return packet;
}

void CActivity::BroadcastActivityScore(Player *player, int32_t score)
{
    if (!player) return;
    
    for (auto pMap : m_activityMaps)
    {
        if (pMap && IsActivityMap(pMap))
        {
            Answer::NetPacket* packet = CreateScorePacket(player, score);
            if (packet)
            {
                pMap->broadcast(packet);
            }
        }
    }
}

void CActivity::BroadcastActivityState()
{
    int32_t nIconState = GetIconState();
    
    if (nIconState == 4 && IsCrossActivity())
    {
        nIconState = 0;
    }
    
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2CC3);
    
    if (!packet) return;
    
    packet->writeInt32(m_cfgActivity->iconid);
    packet->writeInt8(nIconState);
    packet->writeInt32(GetLeftTime());
    packet->writeInt8(0);
    packet->writeInt32(0);
    packet->writeInt8((nIconState == 2) ? 1 : 0);
    packet->setSize(packet->getWOffset());
    
    gameService->worldBroadcast(packet);
}

void CActivity::CheckActivity()
{
    ACTIVITY_STATE nOldState = m_nState;
    
    if (!CheckBeginTimeValid() || !CheckEndTimeValid())
    {
        m_nState = ACTIVITY_STATE::AS_TIME_OUT;
        return;
    }
    
    if (m_nState == ACTIVITY_STATE::AS_TIME_OUT)
    {
        m_nState = ACTIVITY_STATE::AS_NOT_START;
    }
    
    bool bRightLine = IsRightLine();
    CTimer* timer = CTimer::GetInstance();
    tm localnow = timer->GetLocalNow();
    ACTIVITY_STATE nState = GetActivityStateByTime(&localnow);
    
    switch (nState)
    {
    case ACTIVITY_STATE::AS_READY:
        if (m_nState == ACTIVITY_STATE::AS_END || m_nState == ACTIVITY_STATE::AS_NOT_START)
        {
            m_nState = ACTIVITY_STATE::AS_READY;
            if (bRightLine)
            {
                OnActivityReady();
            }
        }
        break;
        
    case ACTIVITY_STATE::AS_RUNNING:
        if (m_nState <= ACTIVITY_STATE::AS_READY)
        {
            if (bRightLine)
            {
                LOG_INFO("activity start id=%d, state=%d, time=%d-%d %d:%d:%d",
                    m_cfgActivity->id, static_cast<int>(m_nState),
                    localnow.tm_year, localnow.tm_yday,
                    localnow.tm_hour, localnow.tm_min, localnow.tm_sec);
                
                StartActivity();
                OnActivityStart();
            }
            else if (GetType() == 19 && GetActivityTime() > 599)
            {
                m_nState = ACTIVITY_STATE::AS_END;
                OnActivityEnd();
            }
            m_nState = ACTIVITY_STATE::AS_RUNNING;
        }
        break;
        
    case ACTIVITY_STATE::AS_END:
        if (m_nState == ACTIVITY_STATE::AS_RUNNING && bRightLine)
        {
            LOG_INFO("activity end id=%d, state=%d", m_cfgActivity->id, static_cast<int>(m_nState));
            StopActivity();
            OnActivityFinish();
        }
        m_nState = ACTIVITY_STATE::AS_END;
        break;
        
    default:
        if (nState == ACTIVITY_STATE::AS_NOT_START)
        {
            if (m_nState == ACTIVITY_STATE::AS_END)
            {
                m_nState = ACTIVITY_STATE::AS_NOT_START;
            }
            else if (m_nState == ACTIVITY_STATE::AS_RUNNING && bRightLine)
            {
                LOG_INFO("activity stop id=%d", m_cfgActivity->id);
                StopActivity();
                OnActivityFinish();
                m_nState = ACTIVITY_STATE::AS_NOT_START;
            }
        }
        break;
    }
    
    if (bRightLine && m_nState != nOldState)
    {
        OnActivityEnd();
        if (IsCrossActivity())
        {
            BroadcastActivityState();
        }
    }
}

void CActivity::GetIconState(std::list<ShowIcon>& iconList)
{
    int32_t nIconState = GetIconStateInternal();
    
    if (nIconState == 0 && m_cfgActivity->typeId == 22)
    {
        nIconState = 4;
    }
    
    if (nIconState != 4 || !IsCrossActivity())
    {
        ShowIcon icon;
        memset(&icon, 0, sizeof(icon));
        icon.nId = m_cfgActivity->iconid;
        icon.nState = nIconState;
        icon.nLeftTime = GetLeftTime();
        
        if (nIconState == 2)
        {
            icon.nEffects = 1;
        }
        
        if (nIconState == 4 && IsCrossActivity())
        {
            icon.nState = 0;
        }
        
        iconList.push_back(icon);
    }
}

int32_t CActivity::GetNextStartTime()
{
    return GetNextStartTimeInternal();
}

int32_t CActivity::GetRevive(Player *player)
{
    size_t lenth = m_cfgActivity->target_regiona.size();
    
    if (lenth == 1)
    {
        return m_cfgActivity->target_regiona[0];
    }
    
    Answer::Random* random = Answer::Random::GetInstance();
    int32_t index = random->generate(0, static_cast<int32_t>(lenth - 1));
    return m_cfgActivity->target_regiona[index];
}

void CActivity::Init()
{
    if (!IsRightLine()) return;
    
    for (auto mapId : m_cfgActivity->maps)
    {
        if (mapId <= 0) continue;
        
        MapManager* mapMgr = MapManager::GetInstance();
        Map* pMap = mapMgr->GetMap(mapId);
        
        if (pMap && pMap->IsActivityMap())
        {
            CActivityMap* pActivityMap = dynamic_cast<CActivityMap*>(pMap);
            if (pActivityMap)
            {
                m_activityMaps.push_back(pActivityMap);
            }
        }
    }
}

bool CActivity::IsRightTime()
{
    CTimer* timer = CTimer::GetInstance();
    tm localnow = timer->GetLocalNow();
    return GetActivityStateByTime(&localnow) == ACTIVITY_STATE::AS_RUNNING;
}

void CActivity::OnChangeMap(CActivityMap *pMap, Player* player, int32_t nX, int32_t nY)
{
    if (!player || !pMap) return;
    
    if (!pMap->isWalkablePosition(nX, nY)) return;
    
    bool bFind = false;
    for (auto pActivityMap : m_activityMaps)
    {
        if (pActivityMap == pMap)
        {
            bFind = true;
            break;
        }
    }
    
    if (bFind)
    {
        player->switchMap(pMap, nX, nY, 1);
    }
}

void CActivity::OnUpdate(CActivityMap *pMap)
{
    if (!pMap) return;
    
    OnMapUpdate(pMap);
    
    if (NeedCheckRevive())
    {
        CheckRevive(pMap);
    }
    
    if (m_nKickTime > 0 && pMap->getNow() >= m_nKickTime)
    {
        std::list<Player*> tList = m_players;
        for (auto player : tList)
        {
            if (player && player->getMap() == pMap)
            {
                player->leaveActivity();
            }
        }
        
        if (m_players.empty())
        {
            OnPlayersEmpty();
        }
    }
}

void CActivity::SendPlayerActivityInfo(Player *player)
{
    if (!player) return;
    
    int8_t ConnId = player->getConnId();
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2E22);
    
    if (!packet) return;
    
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt32(GetNextStartTime());
    packet->setSize(packet->getWOffset());
    
    gameService->sendPacketTo(player->getConnId(), player->getGateIndex(), packet);
}

void CActivity::SendPlayerActivityScore(Player *player)
{
    if (!player) return;
    
    Answer::NetPacket* packet = CreateScorePacket(player, 0);
    if (packet)
    {
        GameService* gameService = GameService::GetInstance();
        gameService->sendPacketTo(player->getConnId(), player->getGateIndex(), packet);
    }
    
    OnPlayerScoreSent(player);
}

void CActivity::AddActivityBuff(Player *player, int32_t nBuffId, bool bClear)
{
    if (!player) return;
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgBuff* cfgBuff = cfgData->getBuff(nBuffId);
    
    if (!cfgBuff) return;
    
    LauncherInfo launcher;
    launcher.id = player->getCid();
    launcher.type = 1;
    
    Buff* buff = new Buff(nBuffId, cfgBuff->level, player, nullptr);
    if (buff)
    {
        player->addBuff(buff);
    }
}

void CActivity::AddPlayer(Player *player)
{
    if (!player) return;
    
    int32_t Now = player->getNow();
    player->SetStartProtect(Now);
    
    int32_t id = m_cfgActivity->id;
    int8_t typeId = m_cfgActivity->typeId;
    
    DailyActivity* playerDailyActivity = player->GetPlayerDailyActivity();
    playerDailyActivity->AddActivityRecord(typeId, id);
    
    m_players.push_back(player);
    OnPlayerAdded(player);
}

void CActivity::AdjustMonsterAttr(CfgMonster* cfgMonster, int32_t nLevel)
{
    if (!cfgMonster) return;
    
    int32_t mid = cfgMonster->mid;
    CfgData* cfgData = CfgData::GetInstance();
    const CfgAdjust* pCfgAdjust = cfgData->GetCfgAdjust(mid, nLevel, true);
    
    if (pCfgAdjust)
    {
        cfgMonster->level = pCfgAdjust->level;
        for (int i = 0; i <= 49; ++i)
        {
            cfgMonster->vAttr[i] = pCfgAdjust->vAttr[i];
        }
    }
}

bool CActivity::CanEnter(Player *player, CActivityMap* pTargetMap)
{
    if (!player || !pTargetMap) return false;
    
    if (m_cfgActivity->line != 0)
    {
        GameService* gameService = GameService::GetInstance();
        if (m_cfgActivity->line != gameService->getLine())
        {
            return false;
        }
    }
    
    int32_t level = m_cfgActivity->level;
    if (level > player->GetLevel())
    {
        return false;
    }
    
    return true;
}

bool CActivity::CheckData()
{
    if (m_cfgActivity->typeId == 24)
    {
        CfgData* cfgData = CfgData::GetInstance();
        // 检查服务器类型
    }
    else if (m_cfgActivity->typeId == 11)
    {
        CfgData* cfgData = CfgData::GetInstance();
        // 检查服务器类型
    }
    
    int32_t end_date = m_cfgActivity->end_date;
    int32_t begin_date = m_cfgActivity->begin_date;
    
    CTimer* timer = CTimer::GetInstance();
    return timer->BetweenDate(begin_date, end_date);
}

bool CActivity::CheckLine()
{
    if (m_cfgActivity->line == 0) return true;
    
    GameService* gameService = GameService::GetInstance();
    return m_cfgActivity->line == gameService->getLine();
}

void CActivity::CheckRevive(CActivityMap *pMap)
{
    if (!pMap) return;
    
    int64_t Tick = pMap->getTick();
    
    if (Tick - m_nLastReviveCheckTick <= 499) return;
    
    m_nLastReviveCheckTick = Tick;
    
    std::list<Player*> tList = m_players;
    for (auto player : tList)
    {
        if (player && player->IsDead())
        {
            int64_t v3 = Tick - player->GetDieTick();
            int32_t reviveTime = GetReviveTime(player);
            
            if (v3 >= reviveTime)
            {
                player->FullHPAndSync();
                player->moveToReviveRegion(1);
                pMap->OnPlayerRevive(player, 1);
            }
        }
    }
}

bool CActivity::CheckWeek()
{
    CTimer* timer = CTimer::GetInstance();
    int32_t weekday = timer->GetWeekDay();
    
    auto it = std::find(m_cfgActivity->weekday.begin(), m_cfgActivity->weekday.end(), weekday);
    return it != m_cfgActivity->weekday.end();
}

void CActivity::DelayKickAll(int32_t nTime)
{
    CTimer* timer = CTimer::GetInstance();
    m_nKickTime = nTime + timer->GetNow();
}

int64_t CActivity::GetActivityTime()
{
    return 60 * m_cfgActivity->duration - GetLeftTime();
}

int32_t CActivity::GetLeftTime()
{
    if (m_cfgActivity->start_hour.empty()) return 0;
    
    CTimer* timer = CTimer::GetInstance();
    int32_t nNowTime = timer->GetNow();
    
    if (m_nState != ACTIVITY_STATE::AS_RUNNING)
    {
        return GetNextStartTimeInternal() - nNowTime;
    }
    
    int32_t startMinute = m_cfgActivity->start_hour[0];
    
    for (size_t i = 0; i < m_cfgActivity->start_hour.size(); ++i)
    {
        int32_t dayZero = DayTime::dayzero(nNowTime);
        int32_t endTime = dayZero + 60 * (m_cfgActivity->start_hour[i] + m_cfgActivity->duration);
        
        if (endTime > nNowTime)
        {
            startMinute = m_cfgActivity->start_hour[i];
            return endTime - nNowTime;
        }
    }
    
    return DayTime::dayzero(nNowTime) + 60 * (startMinute + m_cfgActivity->duration) - nNowTime;
}

int32_t CActivity::GetNextStartTimeInternal()
{
    CTimer* timer = CTimer::GetInstance();
    int32_t nNowTime = timer->GetNow();
    tm localnow = timer->GetLocalNow();
    
    int32_t nowMinute = 60 * localnow.tm_hour + localnow.tm_min;
    int32_t startMinute = m_cfgActivity->start_hour[0];
    
    for (size_t i = 0; i < m_cfgActivity->start_hour.size(); ++i)
    {
        int32_t startTime = DayTime::dayzero(nNowTime) + 60 * m_cfgActivity->start_hour[i];
        if (startMinute > nNowTime)
        {
            startMinute = m_cfgActivity->start_hour[i];
            break;
        }
    }
    
    int32_t days = -1;
    
    if (CheckBeginTimeValid() && CheckEndTimeValid() && nowMinute < startMinute)
    {
        days = 0;
    }
    else if (!m_cfgActivity->weekday.empty())
    {
        int32_t weekday = timer->GetWeekDay();
        int32_t nextWeekday = 0;
        
        for (auto it = m_cfgActivity->weekday.rbegin(); it != m_cfgActivity->weekday.rend(); ++it)
        {
            if (*it > weekday)
            {
                nextWeekday = *it;
                break;
            }
        }
        
        if (nextWeekday == 0)
        {
            nextWeekday = m_cfgActivity->weekday.back();
        }
        
        if (nextWeekday >= weekday)
        {
            days = nextWeekday - weekday;
        }
        else
        {
            days = nextWeekday + 7 - weekday;
        }
    }
    
    return 86400 * days + 60 * startMinute + DayTime::dayzero(nNowTime);
}

bool CActivity::NeedBroadcastActivityScore()
{
    if (m_nBraodcastActivityScoreSign <= 0)
    {
        return false;
    }
    --m_nBraodcastActivityScoreSign;
    return true;
}

void CActivity::OnTimeEnd()
{
    m_nState = ACTIVITY_STATE::AS_END;
    DelayKickAll(30);
}

void CActivity::RemoveActivityMonster(int32_t monsterId)
{
    for (auto mapId : m_cfgActivity->maps)
    {
        MapManager* mapMgr = MapManager::GetInstance();
        Map* pMap = mapMgr->GetMap(mapId);
        if (pMap)
        {
            pMap->delMonster(monsterId);
        }
    }
}

void CActivity::RemovePlayer(Player *player, bool islogout)
{
    for (auto it = m_players.begin(); it != m_players.end(); ++it)
    {
        if (*it == player)
        {
            m_players.erase(it);
            return;
        }
    }
}

void CActivity::Reset()
{
    m_nStartTime = 0;
    m_nKickTime = 0;
    m_nBraodcastActivityScoreSign = 0;
    m_nBroadcastActivityScoreTick = 0;
    m_nLastReviveCheckTick = 0;
    m_players.clear();
}

void CActivity::SendSocialUpdateActivityState(int8_t nState)
{
    GameService* gameService = GameService::GetInstance();
    if (gameService->getLine() != 1) return;
    
    Answer::NetPacket* packet = gameService->popNetpacket(0, Answer::PackType::PACK_DISPATCH, 0x4E52);
    if (!packet) return;
    
    packet->writeInt8(nState);
    packet->setSize(packet->getWOffset());
    gameService->sendPacket(0, packet);
}

void CActivity::SetNeedBroadcastActivityScore(bool b)
{
    if (b)
    {
        m_nBraodcastActivityScoreSign = static_cast<int32_t>(m_activityMaps.size());
    }
    else
    {
        m_nBraodcastActivityScoreSign = 0;
    }
}

void CActivity::StartActivity()
{
    OnPlayersEmpty();
    
    CTimer* timer = CTimer::GetInstance();
    m_nStartTime = timer->GetNow();
    m_nState = ACTIVITY_STATE::AS_RUNNING;
    
    for (auto pMap : m_activityMaps)
    {
        if (pMap)
        {
            int32_t RunnerId = pMap->GetRunnerId();
            MapManager* mapMgr = MapManager::GetInstance();
            mapMgr->PostMsg(RunnerId, GameMsgCode::GMC_ACTIVITY_START, pMap, this, 0, 0, 0);
        }
    }
    
    OnActivityStarted();
}

void CActivity::StopActivity()
{
    m_nState = ACTIVITY_STATE::AS_END;
    
    for (auto pMap : m_activityMaps)
    {
        if (pMap)
        {
            int32_t RunnerId = pMap->GetRunnerId();
            MapManager* mapMgr = MapManager::GetInstance();
            mapMgr->PostMsg(RunnerId, GameMsgCode::GMC_ACTIVITY_STOP, pMap, this, 0, 0, 0);
        }
    }
}

// 虚函数默认实现（供子类重写）
int32_t CActivity::GetType() { return m_cfgActivity->typeId; }
int32_t CActivity::GetId() { return m_cfgActivity->id; }
bool CActivity::IsRuning() { return m_nState == ACTIVITY_STATE::AS_RUNNING; }
bool CActivity::IsCrossActivity() { return false; }
bool CActivity::IsRightLine() { return CheckLine(); }
bool CActivity::IsActivityMap(CActivityMap* pMap) { return pMap != nullptr; }
ACTIVITY_STATE CActivity::GetActivityStateByTime(tm* time) { return ACTIVITY_STATE::AS_NOT_START; }
int32_t CActivity::GetIconState() { return GetIconStateInternal(); }
int32_t CActivity::GetIconStateInternal() { return 0; }
int32_t CActivity::GetReviveTime(Player* player) { return 0; }
void CActivity::OnActivityReady() {}
void CActivity::OnActivityStart() {}
void CActivity::OnActivityFinish() {}
void CActivity::OnActivityEnd() {}
void CActivity::OnActivityStarted() {}
void CActivity::OnMapUpdate(CActivityMap* pMap) {}
void CActivity::OnPlayersEmpty() {}
void CActivity::OnPlayerAdded(Player* player) {}
void CActivity::OnPlayerScoreSent(Player* player) {}