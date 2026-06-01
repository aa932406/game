#include "Other/CRefreshMonster.h"
#include "Answer.h"
#include "GameService.h"
#include "MapManager.h"
#include "CTimer.h"
#include "CfgData.h"

CRefreshMonster::CRefreshMonster(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    std::list<RefreshMonsterCfg> cfgList;
    CfgData::GetRefreshMonsterCfgList(cfgData, &cfgList);
    m_RefreshCfgList = cfgList;
}

CRefreshMonster::~CRefreshMonster()
{
}

void CRefreshMonster::reset()
{
    for (auto& cfg : m_RefreshCfgList)
        cfg.LastFreshTime = 0;
    m_StateTime = 0;
    CActivity::reset();
}

void CRefreshMonster::CheckActivity()
{
    if (!IsRightLine())
    {
        m_nState = ACTIVITY_STATE::AS_TIME_OUT;
    }
    else if (!CheckData())
    {
        m_nState = ACTIVITY_STATE::AS_NOT_START;
    }
    else
    {
        if (m_nState == ACTIVITY_STATE::AS_TIME_OUT)
            m_nState = ACTIVITY_STATE::AS_NOT_START;

        CTimer* timer = Answer::Singleton<CTimer>::instance();
        tm localnow;
        CTimer::GetLocalNow(&localnow, timer);
        int32_t nowTime = CTimer::GetNow(timer);
        bool bRightLine = IsRightLine();
        ACTIVITY_STATE nState = GetActivityStateByTime(&localnow);

        if (nState == ACTIVITY_STATE::AS_READY)
        {
            if (m_nState == ACTIVITY_STATE::AS_END || m_nState == ACTIVITY_STATE::AS_NOT_START)
            {
                m_nState = ACTIVITY_STATE::AS_READY;
                if (bRightLine)
                {
                    OnActivityReady();
                    BroadcastActivityState();
                }
            }
        }
        else if (nState == ACTIVITY_STATE::AS_RUNNING)
        {
            if (m_nState <= ACTIVITY_STATE::AS_READY && bRightLine)
            {
                broadcastStart();
                OnActivityStart();
                BroadcastActivityState();
                m_StateTime = nowTime;
            }
            m_nState = ACTIVITY_STATE::AS_RUNNING;
            if (bRightLine)
                FlushMonsters(nowTime);
        }
        else if (nState == ACTIVITY_STATE::AS_END)
        {
            if (m_nState == ACTIVITY_STATE::AS_RUNNING)
            {
                if (bRightLine)
                {
                    OnTimeEnd();
                    BroadcastActivityState();
                }
                m_nState = ACTIVITY_STATE::AS_END;
            }
            m_nState = ACTIVITY_STATE::AS_END;
        }
        else if (nState == ACTIVITY_STATE::AS_NOT_START)
        {
            if (m_nState == ACTIVITY_STATE::AS_END)
            {
                m_nState = ACTIVITY_STATE::AS_NOT_START;
            }
            else if (m_nState == ACTIVITY_STATE::AS_RUNNING)
            {
                if (bRightLine)
                {
                    OnTimeEnd();
                    BroadcastActivityState();
                }
                m_nState = ACTIVITY_STATE::AS_NOT_START;
            }
        }
    }
}

void CRefreshMonster::FlushMonsters(int32_t nowTime)
{
    for (auto& cfg : m_RefreshCfgList)
    {
        if (m_StateTime + cfg.nStartTime < nowTime)
        {
            if (cfg.LastFreshTime <= 0 || cfg.LastFreshTime + cfg.nDiffTime <= nowTime)
            {
                cfg.LastFreshTime = nowTime;
                int32_t leftTime = cfg.nDiffTime <= getLeftTime() ? cfg.nDiffTime : getLeftTime();

                for (auto mapId : m_cfgActivity->maps)
                {
                    MapManager* mm = Answer::Singleton<MapManager>::instance();
                    Map* pMap = MapManager::GetMap(mm, mapId);
                    if (pMap)
                    {
                        int32_t runnerId = pMap->getRunnerId();
                        MapManager* mm2 = Answer::Singleton<MapManager>::instance();
                        MapManager::PostMsg(mm2, runnerId, GameMsgCode::GMC_ADD_OBJ, pMap, this, leftTime, cfg.nIndex, 0);
                        FreshObjGongGao(cfg.GongGaoId);
                    }
                }
            }
        }
    }
}

void CRefreshMonster::stopActivity()
{
    m_nState = ACTIVITY_STATE::AS_NOT_START;
    OnActivityEnd();
}

void CRefreshMonster::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(m_cfgActivity->StartGongGao);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CRefreshMonster::FreshObjGongGao(int32_t gongGaoId)
{
    if (gongGaoId > 0)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(gongGaoId);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, 0, packet);
        }
    }
}
