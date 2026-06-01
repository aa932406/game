// CExtCharTeam.cpp
#include "CExtCharTeam.h"
#include <cstring>

CExtCharTeam::CExtCharTeam()
{
    m_nTeamId = 0;
    m_nLastQueryTeamTick = 0;
    m_nLeaderId = 0;
    m_strLeaderName.clear();
    m_lstMember.clear();
    OnCleanUp();
}

CExtCharTeam::~CExtCharTeam()
{
}

void CExtCharTeam::OnCleanUp()
{
    m_nTeamId = 0;
    m_nLastQueryTeamTick = 0;
    m_nLeaderId = 0;
    m_strLeaderName.clear();
    m_lstMember.clear();
}

void CExtCharTeam::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 720;
    ProcId_t v4 = 20067;
    procList->push_back(v3);
    procList->push_back(v4);
}

int32_t CExtCharTeam::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 2;
    if (!IsFunctionOpen()) return 2;
    
    switch (nProcId)
    {
        case 720: return onSetTeamAutoOperate(inPacket);
        case 20067: return onUpdateTeamInfo(inPacket);
        case 46: return onQueryTeamsAround(inPacket);
        default: return 0;
    }
}

int32_t CExtCharTeam::onQueryTeamsAround(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharTeam::onSetTeamAutoOperate(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    return 0;
}

int32_t CExtCharTeam::onUpdateTeamInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 2;
    return 0;
}

void CExtCharTeam::Broadcast(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return;
}

void CExtCharTeam::LeaveTeam()
{
    if (!m_pPlayer || !IsInTeam()) return;
}

void CExtCharTeam::BroadcastBasicData()
{
    if (!m_pPlayer || !IsInTeam()) return;
}

void CExtCharTeam::UpdateTeamSetting()
{
    if (!m_pPlayer) return;
}

bool CExtCharTeam::IsInTeam()
{
    return (m_nTeamId != 0);
}

bool CExtCharTeam::IsFunctionOpen()
{
    return true;
}

void CExtCharTeam::OnUpdate(int64_t curTick)
{
}

void CExtCharTeam::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharTeam::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharTeam::OnSaveToDB(PlayerDBData* dbData)
{
}