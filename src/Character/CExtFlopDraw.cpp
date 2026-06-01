// CExtFlopDraw.cpp
#include "CExtFlopDraw.h"
#include <cstring>

CExtFlopDraw::CExtFlopDraw()
{
    OnCleanUp();
}

CExtFlopDraw::~CExtFlopDraw()
{
}

void CExtFlopDraw::OnCleanUp()
{
    m_FlopDrawRecordMap.clear();
}

void CExtFlopDraw::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_FlopDrawRecordMap = dbData->m_CFlopDraw.m_FlopDrawRecordMap;
    }
}

void CExtFlopDraw::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_CFlopDraw.m_FlopDrawRecordMap = m_FlopDrawRecordMap;
    }
}

bool CExtFlopDraw::OnFlopDraw(int32_t nType, int32_t nFlopId)
{
    if (!m_pPlayer) return 0;
    return 0;
}

int32_t CExtFlopDraw::GetFlopType(int32_t nType, int32_t nFlopId)
{
    return 0;
}

bool CExtFlopDraw::DrawAndCost(int32_t nType, int32_t nFlopId)
{
    return 0;
}

bool CExtFlopDraw::IsGet(int32_t nType, int32_t nId)
{
    auto it = m_FlopDrawRecordMap.find(nType);
    if (it != m_FlopDrawRecordMap.end())
    {
        for (auto& pair : it->second)
        {
            if (pair.second == nId) return true;
        }
    }
    return false;
}

bool CExtFlopDraw::IsFloped(int32_t nType, int32_t nFlopId)
{
    auto it = m_FlopDrawRecordMap.find(nType);
    if (it != m_FlopDrawRecordMap.end())
    {
        return (it->second.find(nFlopId) != it->second.end());
    }
    return false;
}

void CExtFlopDraw::addDrawTimes(int32_t nType, int32_t nFlopId, int32_t FlopResult)
{
    m_FlopDrawRecordMap[nType][nFlopId] = FlopResult;
}

void CExtFlopDraw::SendFlopRecord(Player* player, int32_t nType)
{
    if (!player) return;
}

void CExtFlopDraw::SendFlopOneRecord(Player* player, int32_t nType, int32_t nFlopId, int32_t FlopResult)
{
    if (!player) return;
}

void CExtFlopDraw::OnUpdate(int64_t curTick)
{
}

void CExtFlopDraw::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtFlopDraw::GetInterestsProtocol(ProcIdList* procList)
{
}