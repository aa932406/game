// CExtCharWing.cpp
#include "CExtCharWing.h"
#include "Database/PlayerDBData.h"
#include <cstring>

CExtCharWing::CExtCharWing()
{
    OnCleanUp();
}

CExtCharWing::~CExtCharWing()
{
}

void CExtCharWing::OnCleanUp()
{
    m_Level = 0;
    m_Luck = 0;
    m_LaseUpdateTick = 0;
    m_HuanHua = 0;
}

void CExtCharWing::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_Level = dbData->m_CharWingDBData.m_Level;
        m_Luck = dbData->m_CharWingDBData.m_Luck;
        m_HuanHua = dbData->m_CharWingDBData.m_HuanHua;
        if (!m_HuanHua) m_HuanHua = m_Level;
    }
}

void CExtCharWing::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_CharWingDBData.m_Level = m_Level;
        dbData->m_CharWingDBData.m_Luck = m_Luck;
        dbData->m_CharWingDBData.m_HuanHua = m_HuanHua;
    }
}

void CExtCharWing::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 761;
    ProcId_t v4 = 762;
    procList->push_back(v3);
    procList->push_back(v4);
}

int32_t CExtCharWing::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 10002;
    if (!IsFunctionOpen()) return 10002;
    
    switch (nProcId)
    {
        case 761: return onWingUpLevel(inPacket);
        case 762: return onWingHuanHua(inPacket);
        case 760: return onRequestInfo(inPacket);
        default: return 0;
    }
}

int32_t CExtCharWing::onWingHuanHua(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharWing::onRequestInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    SendWingInfo();
    return 0;
}

int32_t CExtCharWing::onWingUpLevel(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

void CExtCharWing::OnDaySwitch(int32_t nDiffDays)
{
    ClearLuck();
}

void CExtCharWing::ClearLuck()
{
    if (!m_pPlayer) return;
    m_Luck = 0;
    SendWingInfo();
}

void CExtCharWing::SendWingInfo()
{
    if (!m_pPlayer) return;
}

void CExtCharWing::AddCharAttr()
{
    if (!m_pPlayer) return;
    if (!IsFunctionOpen()) return;
}

bool CExtCharWing::IsFunctionOpen()
{
    return true;
}

void CExtCharWing::OnUpdate(int64_t curTick)
{
}