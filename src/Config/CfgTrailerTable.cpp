#include "Config/CfgTrailerTable.h"
#include "Answer/Logger.h"
#include <map>

std::map<int32_t, CfgTrailer> CfgTrailerTable::m_mTrailer;

CfgTrailerTable::CfgTrailerTable()
{
    m_mTrailer.clear();
}

CfgTrailerTable::~CfgTrailerTable()
{
    m_mTrailer.clear();
}

void CfgTrailerTable::Add(const CfgTrailer& stu)
{
    if (stu.nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTrailerTable::Add invalid trailer id: %d", stu.nId);
        return;
    }
    
    m_mTrailer[stu.nId] = stu;
}

const CfgTrailer* CfgTrailerTable::GetTrailer(int32_t nId)
{
    auto it = m_mTrailer.find(nId);
    if (it != m_mTrailer.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgTrailerTable::IsTrailerExist(int32_t nId)
{
    return m_mTrailer.find(nId) != m_mTrailer.end();
}

int32_t CfgTrailerTable::GetTrailerQuality(int32_t nId)
{
    const CfgTrailer* pTrailer = GetTrailer(nId);
    if (pTrailer)
    {
        return pTrailer->TrailerQuality;
    }
    return 0;
}

int32_t CfgTrailerTable::GetTrailerTaskId(int32_t nId)
{
    const CfgTrailer* pTrailer = GetTrailer(nId);
    if (pTrailer)
    {
        return pTrailer->nTaskId;
    }
    return 0;
}

void CfgTrailerTable::Clear()
{
    m_mTrailer.clear();
}