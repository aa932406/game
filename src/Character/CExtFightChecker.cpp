// CExtFightChecker.cpp
#include "CExtFightChecker.h"
#include <cstring>

CExtFightChecker::CExtFightChecker()
{
    OnCleanUp();
}

CExtFightChecker::~CExtFightChecker()
{
}

void CExtFightChecker::OnCleanUp()
{
    m_bIsInFight = 0;
    m_IsBuleName = 0;
    m_bIsInPvP = 0;
    m_nFightTick = 0;
    m_nBuleNameTick = 0;
    m_nPvPTick = 0;
}

void CExtFightChecker::OnUpdate(int64_t curTick)
{
    CheckInFight(curTick);
}

void CExtFightChecker::SetInPvP(int64_t nCurTick)
{
    if (IsBuleName())
    {
        m_nBuleNameTick = nCurTick;
    }
    m_nPvPTick = nCurTick;
}

void CExtFightChecker::CheckInFight(int64_t nCurTick)
{
    if (nCurTick - m_nFightTick <= 15000)
    {
        if (!m_bIsInFight) OnEnterFight();
    }
    else if (m_bIsInFight)
    {
        OnLeaveFight();
        m_nBuleNameTick = nCurTick;
    }
    
    if (nCurTick - m_nPvPTick <= 15000)
    {
        if (!m_bIsInPvP) enterPvP();
    }
    else if (m_bIsInPvP)
    {
        leavePvP();
    }
    
    if (!m_bIsInPvP && nCurTick - m_nBuleNameTick > 15000 && IsBuleName())
    {
        ChangeBuleName(0);
    }
}

void CExtFightChecker::OnLeaveFight()
{
    m_bIsInFight = 0;
    UpdateFightState();
}

void CExtFightChecker::OnEnterFight()
{
    m_bIsInFight = 1;
    UpdateFightState();
}

void CExtFightChecker::ChangeBuleName(bool bFlag)
{
    if (!m_pPlayer) return;
    
    if (m_IsBuleName != bFlag)
    {
        m_IsBuleName = bFlag;
    }
    m_nBuleNameTick = 0;
}

bool CExtFightChecker::IsBuleName()
{
    return m_IsBuleName != 0;
}

void CExtFightChecker::enterPvP()
{
    m_bIsInPvP = 1;
    UpdatePvPState();
}

void CExtFightChecker::leavePvP()
{
    m_bIsInPvP = 0;
    UpdatePvPState();
}

void CExtFightChecker::UpdateFightState()
{
}

void CExtFightChecker::UpdatePvPState()
{
}

void CExtFightChecker::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtFightChecker::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtFightChecker::OnSaveToDB(PlayerDBData* dbData)
{
}

void CExtFightChecker::GetInterestsProtocol(ProcIdList* procList)
{
}