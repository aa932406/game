// CExtCharPortal.cpp
#include "CExtCharPortal.h"
#include "Database/PlayerDBData.h"
#include <cstring>

CExtCharPortal::CExtCharPortal()
{
    OnCleanUp();
}

CExtCharPortal::~CExtCharPortal()
{
}

void CExtCharPortal::OnCleanUp()
{
    m_nPortalId = 0;
    m_lstPortal.clear();
    m_nLastTick = 0;
}

void CExtCharPortal::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_lstPortal = dbData->m_PortalDBData.lstPortal;
        m_nPortalId = dbData->m_PortalDBData.nPortalId;
        checkPortalTime(1);
    }
}

void CExtCharPortal::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_PortalDBData.lstPortal = m_lstPortal;
    }
}

void CExtCharPortal::OnUpdate(int64_t curTick)
{
    if (m_pPlayer)
    {
        if (curTick - m_nLastTick > 999)
        {
            checkPortalTime(0);
            m_nLastTick = curTick;
        }
    }
}

void CExtCharPortal::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 810;
    procList->push_back(v3);
}

int32_t CExtCharPortal::onRequestInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    SendPortalInfo(&m_lstPortal);
    return 0;
}

int32_t CExtCharPortal::onUsePortal(Answer::NetPacket* inPacket)
{
    int32_t nId = 0;
    
    if (!m_pPlayer || !inPacket) return 10002;
    
    return 0;
}

void CExtCharPortal::SendPortalInfo(const PortalInfo* info)
{
    if (!m_pPlayer) return;
}

void CExtCharPortal::SendPortalInfo(const PortalInfoList* lst)
{
    if (!m_pPlayer) return;
}

void CExtCharPortal::SendPortalClose(int32_t nId)
{
    if (!m_pPlayer) return;
}

int32_t CExtCharPortal::getPortalId()
{
    if (++m_nPortalId > 9999) m_nPortalId = 1;
    return m_nPortalId;
}

void CExtCharPortal::checkPortalTime(bool bInit)
{
    if (!m_pPlayer) return;
}

void CExtCharPortal::BroadcastPortal()
{
    if (!m_pPlayer) return;
}

void CExtCharPortal::OnDaySwitch(int32_t nDiffDays)
{
}