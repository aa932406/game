#include "Config/CfgTitleTable.h"
#include "Answer/Logger.h"
#include <map>
#include <vector>

CfgTitleTable::CfgTitleTable()
{
    m_mTitle.clear();
    m_mTitleByType.clear();
}

CfgTitleTable::~CfgTitleTable()
{
    m_mTitle.clear();
    m_mTitleByType.clear();
}

void CfgTitleTable::Add(const CfgTitle& stu)
{
    if (stu.nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTitleTable::Add invalid title id: %d", stu.nId);
        return;
    }
    
    m_mTitle[stu.nId] = stu;
    m_mTitleByType[stu.nType].push_back(stu.nId);
}

const CfgTitle* CfgTitleTable::GetTitle(int32_t nId)
{
    auto it = m_mTitle.find(nId);
    if (it != m_mTitle.end())
    {
        return &it->second;
    }
    return nullptr;
}

const std::vector<int32_t>* CfgTitleTable::GetTitleIdsByType(int32_t nType)
{
    auto it = m_mTitleByType.find(nType);
    if (it != m_mTitleByType.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgTitleTable::IsTitleExist(int32_t nId)
{
    return m_mTitle.find(nId) != m_mTitle.end();
}

int32_t CfgTitleTable::GetTitlePriority(int32_t nId)
{
    const CfgTitle* pTitle = GetTitle(nId);
    if (pTitle)
    {
        return pTitle->nPriority;
    }
    return 0;
}

int32_t CfgTitleTable::GetTitleType(int32_t nId)
{
    const CfgTitle* pTitle = GetTitle(nId);
    if (pTitle)
    {
        return pTitle->nType;
    }
    return 0;
}

void CfgTitleTable::Clear()
{
    m_mTitle.clear();
    m_mTitleByType.clear();
}