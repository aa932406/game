// CExtCharJueWei.cpp
#include "CExtCharJueWei.h"
#include "Database/PlayerDBData.h"
#include <cstring>

CExtCharJueWei::CExtCharJueWei()
{
    m_nJueWei = 0;
}

CExtCharJueWei::~CExtCharJueWei()
{
}

void CExtCharJueWei::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 667;
    procList->push_back(v3);
}

int32_t CExtCharJueWei::onRequestJueWeiInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    sendJueWeiInfo();
    return 0;
}

int32_t CExtCharJueWei::onAskJueWeiLevelUp(Answer::NetPacket* inPacket)
{
    int32_t v2 = 0;
    
    if (!inPacket || !m_pPlayer) return 10002;
    
    return 0;
}

void CExtCharJueWei::GongGao()
{
    if (!m_pPlayer) return;
}

void CExtCharJueWei::AddCharAttr()
{
    if (!m_pPlayer) return;
}

void CExtCharJueWei::sendJueWeiInfo()
{
    if (!m_pPlayer) return;
}

void CExtCharJueWei::OnCleanUp()
{
    m_nJueWei = 0;
}

void CExtCharJueWei::OnUpdate(int64_t curTick)
{
}

void CExtCharJueWei::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharJueWei::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_nJueWei = dbData->m_nJueWei;
    }
}

void CExtCharJueWei::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_nJueWei = m_nJueWei;
    }
}