#include "common.h"
#include "Other/CMoLingRuQin.h"
#include "Answer.h"
#include "GameService.h"
#include "CTimer.h"
#include "CfgData.h"
#include "MapManager.h"

CMoLingRuQin::CMoLingRuQin(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_LastFlushTime = 0;
    m_refreshCount = 1;
    m_RefreshTime = 0;
}

CMoLingRuQin::~CMoLingRuQin()
{
}

void CMoLingRuQin::CheckActivity()
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
        CTimer* timer = Answer::Singleton<CTimer>::instance();
        tm localnow;
        CTimer::GetLocalNow(&localnow, timer);
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
        else if (nState > ACTIVITY_STATE::AS_READY)
        {
            if (nState == ACTIVITY_STATE::AS_RUNNING)
            {
                if (m_nState <= ACTIVITY_STATE::AS_READY && bRightLine)
                {
                    broadcastStart();
                    OnActivityStart();
                    BroadcastActivityState();
                }
                if (bRightLine)
                    FlushMonsters();
                m_nState = ACTIVITY_STATE::AS_RUNNING;
            }
            else if (nState == ACTIVITY_STATE::AS_END)
            {
                if (m_nState == ACTIVITY_STATE::AS_RUNNING)
                {
                    if (bRightLine)
                    {
                        onTimeEnd();
                        BroadcastActivityState();
                    }
                    m_nState = ACTIVITY_STATE::AS_END;
                }
                m_nState = ACTIVITY_STATE::AS_END;
                m_refreshCount = 1;
                m_RefreshTime = 0;
            }
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
                    onTimeEnd();
                    BroadcastActivityState();
                }
                m_nState = ACTIVITY_STATE::AS_NOT_START;
            }
            m_refreshCount = 1;
            m_RefreshTime = 0;
        }
    }
}

void CMoLingRuQin::FlushMonsters()
{
    CTimer* timer = Answer::Singleton<CTimer>::instance();
    int32_t nowTime = CTimer::GetNow(timer);
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    const cfgGoblinTableData* goblinTable = CfgData::GetGoblinTable(cfgData);
    const cfgGoblinData* goblin = cfgGoblinTableData::getGoblinData(goblinTable, m_refreshCount);

    if (goblin && nowTime - m_LastFlushTime >= m_RefreshTime)
    {
        for (auto mapId : m_cfgActivity->maps)
        {
            MapManager* mm = Answer::Singleton<MapManager>::instance();
            Map* pMap = MapManager::GetMap(mm, mapId);
            if (pMap)
            {
                int32_t runnerId = pMap->getRunnerId();
                MapManager* mm2 = Answer::Singleton<MapManager>::instance();
                MapManager::PostMsg(mm2, runnerId, GameMsgCode::GMC_GOBLIN_ACTIVITY, pMap, this, m_refreshCount, 0, 0);
            }
        }
        m_LastFlushTime = nowTime;
        ++m_refreshCount;
        m_RefreshTime = goblin->m_nRefreshTime;
    }
}

void CMoLingRuQin::startActivity()
{
    m_nState = ACTIVITY_STATE::AS_RUNNING;
    FlushMonsters();
}

void CMoLingRuQin::stopActivity()
{
    m_nState = ACTIVITY_STATE::AS_NOT_START;
    OnActivityEnd();
}

void CMoLingRuQin::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(329);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CMoLingRuQin::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(327);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CMoLingRuQin::onTimeEnd()
{
    for (auto mapId : m_cfgActivity->maps)
    {
        MapManager* mm = Answer::Singleton<MapManager>::instance();
        Map* pMap = MapManager::GetMap(mm, mapId);
        if (pMap)
        {
            int32_t runnerId = pMap->getRunnerId();
            MapManager* mm2 = Answer::Singleton<MapManager>::instance();
            MapManager::PostMsg(mm2, runnerId, GameMsgCode::GMC_GOBLIN_ACTIVITY, pMap, this, 0, 0, 0);
        }
    }
}
