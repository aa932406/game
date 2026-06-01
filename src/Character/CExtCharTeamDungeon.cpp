// CExtCharTeamDungeon.cpp
#include "CExtCharTeamDungeon.h"
#include <cstring>

CExtCharTeamDungeon::CExtCharTeamDungeon()
{
    OnCleanUp();
}

CExtCharTeamDungeon::~CExtCharTeamDungeon()
{
}

void CExtCharTeamDungeon::OnCleanUp()
{
    m_nId = 0;
    m_nDungeonId = 0;
    m_nLeader = 0;
}

void CExtCharTeamDungeon::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t procs[] = {801, 802, 20069, 20070, 20071};
    for (int i = 0; i < 5; ++i)
    {
        procList->push_back(procs[i]);
    }
}

int32_t CExtCharTeamDungeon::onTeamDungeonCreate(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    return 0;
}

int32_t CExtCharTeamDungeon::onTeamDungeonJoin(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    return 0;
}

int32_t CExtCharTeamDungeon::onTeamDungeonStart(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    return 0;
}

void CExtCharTeamDungeon::LeaveTeamDungeon()
{
    if (!IsInTeamDungeon()) return;
}

bool CExtCharTeamDungeon::IsInTeamDungeon()
{
    return (m_nId != 0);
}

bool CExtCharTeamDungeon::checkTeamDungeon(const void* pCfgDungeon)
{
    return true;
}

void CExtCharTeamDungeon::OnUpdate(int64_t curTick)
{
}

void CExtCharTeamDungeon::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharTeamDungeon::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharTeamDungeon::OnSaveToDB(PlayerDBData* dbData)
{
}