#include "Other/CKaiFuBoss.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "CTimer.h"

CKaiFuBoss::CKaiFuBoss(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_Boss = 0;
}

CKaiFuBoss::~CKaiFuBoss()
{
}

void CKaiFuBoss::reset()
{
    m_Boss = 0;
    CActivity::reset();
}

void CKaiFuBoss::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(470);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CKaiFuBoss::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(471);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CKaiFuBoss::broadcastTimeEnd()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        if (m_Boss && Unit::isAlive(m_Boss))
            packet->writeInt32(473);
        else
            packet->writeInt32(472);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CKaiFuBoss::sendPlayerScore(Player* player)
{
    if (player)
    {
        int8_t connId = player->getConnId();
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E26u);
        if (packet)
        {
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt32(getLeftTime());
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = player->getGateIndex();
            int8_t connId2 = player->getConnId();
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}

void CKaiFuBoss::onTimeEnd()
{
    broadcastTimeEnd();
    delayKickAll(0);
}

bool CKaiFuBoss::checkData()
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    if (CfgData::isUniteServer(cfgData))
        return false;
    cfgData = Answer::Singleton<CfgData>::instance();
    int32_t startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);
    if (isSpecialDay(startDays))
        return true;
    return CActivity::checkData();
}

bool CKaiFuBoss::checkWeek()
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    if (CfgData::isUniteServer(cfgData))
        return false;
    cfgData = Answer::Singleton<CfgData>::instance();
    int32_t startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);
    if (isSpecialDay(startDays))
        return true;
    return CActivity::checkWeek();
}

bool CKaiFuBoss::isSpecialDay(int32_t nStartDays)
{
    return (unsigned int)nStartDays <= 6;
}

int32_t CKaiFuBoss::getNextStartTime()
{
    if (IsRuning() || m_cfgActivity->start_hour.empty())
        return 0;

    CTimer* timer = Answer::Singleton<CTimer>::instance();
    int32_t nNowTime = CTimer::GetNow(timer);
    timer = Answer::Singleton<CTimer>::instance();
    tm localnow;
    CTimer::GetLocalNow(&localnow, timer);
    int32_t nowMinute = 60 * localnow.tm_hour + localnow.tm_min;
    int32_t startMinute = m_cfgActivity->start_hour[0];
    int32_t startTime = 0;
    uint32_t i;

    for (i = 0; i < m_cfgActivity->start_hour.size(); ++i)
    {
        int32_t v5 = Answer::DayTime::dayzero(nNowTime);
        startTime = v5 + 60 * m_cfgActivity->start_hour[i];
        if (startMinute > nowMinute)
        {
            startMinute = m_cfgActivity->start_hour[i];
            break;
        }
    }

    int32_t days = -1;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    int32_t startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);

    if (IsRightLine() && checkData())
    {
        if (nowMinute < startMinute)
            days = 0;
    }
    else if (startDays <= 6)
    {
        days = 7 - startDays;
    }

    if (days < 0)
    {
        if (m_cfgActivity->weekday.empty())
            return 0;
        timer = Answer::Singleton<CTimer>::instance();
        int32_t weekday = CTimer::GetWeekDay(timer);
        int32_t nextweekday = 0;

        for (auto rit = m_cfgActivity->weekday.rbegin(); rit != m_cfgActivity->weekday.rend(); ++rit)
        {
            if (*rit > weekday)
            {
                nextweekday = *rit;
                break;
            }
        }
        if (nextweekday == 0)
            nextweekday = m_cfgActivity->weekday.back();

        if (nextweekday >= weekday)
            days = nextweekday - weekday;
        else
            days = nextweekday + 7 - weekday;
    }

    return 86400 * days + 60 * startMinute + Answer::DayTime::dayzero(nNowTime);
}

void CKaiFuBoss::onMonsterAdd(MonsterActivity* pMonster)
{
    if (pMonster)
        m_Boss = pMonster;
}
