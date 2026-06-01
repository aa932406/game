// CExtCharHallOfFame.cpp
#include "CExtCharHallOfFame.h"
#include <cstring>

CExtCharHallOfFame::CExtCharHallOfFame()
{
    OnCleanUp();
}

CExtCharHallOfFame::~CExtCharHallOfFame()
{
}

void CExtCharHallOfFame::OnCleanUp()
{
}

void CExtCharHallOfFame::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t procs[] = {851, 852, 853, 20078, 20079, 20080};
    for (int i = 0; i < 6; ++i)
    {
        procList->push_back(procs[i]);
    }
}

int32_t CExtCharHallOfFame::onFight(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharHallOfFame::onBuyTimes(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharHallOfFame::onClearCD(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharHallOfFame::onRequestInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    sendSocialRequestInfo();
    return 0;
}

int32_t CExtCharHallOfFame::onSocialFightResult(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharHallOfFame::onSocialGetReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharHallOfFame::onSocialRankRewardInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

void CExtCharHallOfFame::sendSocialFight(int32_t nIndex)
{
    if (!m_pPlayer) return;
}

void CExtCharHallOfFame::sendSocialRequestInfo()
{
    if (!m_pPlayer) return;
}

void CExtCharHallOfFame::sendSocialRewardResult(int32_t nIndex, int32_t nResult)
{
    if (!m_pPlayer) return;
}

void CExtCharHallOfFame::addFightTimes()
{
}

void CExtCharHallOfFame::addBuyTimes()
{
}

void CExtCharHallOfFame::addCDTime(bool bWin)
{
}

void CExtCharHallOfFame::clearCDTime()
{
}

void CExtCharHallOfFame::GetIconState(void* iconList)
{
    if (IsFunctionOpen())
    {
        // Add icon
    }
}

void CExtCharHallOfFame::SendIconState(int32_t nReward)
{
    if (!m_pPlayer) return;
}

int32_t CExtCharHallOfFame::getFightTimes()
{
    return 0;
}

int32_t CExtCharHallOfFame::getLimitFightTimes()
{
    return 10;
}

int32_t CExtCharHallOfFame::getBuyTimes()
{
    return 0;
}

int32_t CExtCharHallOfFame::getLimitBuyTimes()
{
    return 5;
}

int32_t CExtCharHallOfFame::getCDTime()
{
    return 0;
}

bool CExtCharHallOfFame::IsFunctionOpen()
{
    return true;
}

void CExtCharHallOfFame::OnUpdate(int64_t curTick)
{
}

void CExtCharHallOfFame::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharHallOfFame::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharHallOfFame::OnSaveToDB(PlayerDBData* dbData)
{
}