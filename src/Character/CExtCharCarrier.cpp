// CExtCharCarrier.cpp
#include "CExtCharCarrier.h"
#include <cstring>

CExtCharCarrier::CExtCharCarrier()
{
    m_pCarrier = 0;
}

CExtCharCarrier::~CExtCharCarrier()
{
}

void CExtCharCarrier::OnCleanUp()
{
    if (m_pCarrier)
    {
        m_pCarrier = 0;
    }
}

void CExtCharCarrier::OnUpdate(int64_t curTick)
{
    if (m_pCarrier)
    {
        // Update carrier
    }
}

void CExtCharCarrier::GetInterestsProtocol(ProcIdList* procList)
{
    // No protocols by default
}

int32_t CExtCharCarrier::onLeaveCarrier(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pCarrier) return 10002;
    LeaveCarrier();
    return 0;
}

int32_t CExtCharCarrier::GetCarrierId()
{
    if (m_pCarrier) return 1;
    return 0;
}

void CExtCharCarrier::EnterCarrier(int32_t nId)
{
    if (!m_pPlayer) return;
    
    if (m_pCarrier)
        LeaveCarrier();
    
    // Create new carrier
}

void CExtCharCarrier::LeaveCarrier()
{
    if (!m_pPlayer) return;
    
    if (m_pCarrier)
    {
        m_pCarrier = 0;
    }
}

bool CExtCharCarrier::HaveSkill(int32_t nSkillId)
{
    return (m_pPlayer && m_pCarrier);
}

bool CExtCharCarrier::IsInCarrier()
{
    return (m_pCarrier != 0);
}

void CExtCharCarrier::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharCarrier::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharCarrier::OnSaveToDB(PlayerDBData* dbData)
{
}