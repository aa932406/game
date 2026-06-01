// CExtCharWorship.cpp
#include "CExtCharWorship.h"
#include <cstring>

CExtCharWorship::CExtCharWorship()
{
    OnCleanUp();
}

CExtCharWorship::~CExtCharWorship()
{
}

void CExtCharWorship::OnCleanUp()
{
    m_nWorshipTimes = 0;
    m_lstChars.clear();
}

void CExtCharWorship::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_nWorshipTimes = dbData->m_WorshipData.nTimes;
        loadCharList(&dbData->m_WorshipData.strCharList);
    }
}

void CExtCharWorship::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_WorshipData.nTimes = m_nWorshipTimes;
        dbData->m_WorshipData.strCharList = saveCharList();
    }
}

void CExtCharWorship::OnDaySwitch(int32_t nDiffDays)
{
    m_nWorshipTimes = 0;
    m_lstChars.clear();
    SendWorshipInfo();
}

void CExtCharWorship::GetInterestsProtocol(ProcIdList* procList)
{
}

int32_t CExtCharWorship::onSocialWorship(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

void CExtCharWorship::sendSocialWorship(CharId_t nCharId)
{
    if (!m_pPlayer) return;
}

void CExtCharWorship::SendWorshipInfo()
{
    if (!m_pPlayer) return;
}

void CExtCharWorship::loadCharList(const std::string* str)
{
    if (!str) return;
    m_lstChars.clear();
    m_nWorshipTimes = 0;
}

std::string CExtCharWorship::saveCharList()
{
    std::string result;
    return result;
}

bool CExtCharWorship::isTimesLimit()
{
    return m_nWorshipTimes >= 10;
}

bool CExtCharWorship::isWorshiped(CharId_t nCharId)
{
    for (auto& cid : m_lstChars)
    {
        if (cid == nCharId) return true;
    }
    return false;
}

void CExtCharWorship::OnUpdate(int64_t curTick)
{
}