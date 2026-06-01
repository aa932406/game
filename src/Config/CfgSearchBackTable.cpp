#include "Config/CfgSearchBackTable.h"
#include "Answer/Logger.h"
#include <map>
#include <vector>

static std::map<int32_t, CfgSearchBack> m_mSearchBack;
static std::map<int32_t, std::vector<CfgSearchBack>> m_mSearchBackByType;

CfgSearchBackTable::CfgSearchBackTable()
{
    m_mSearchBack.clear();
    m_mSearchBackByType.clear();
}

CfgSearchBackTable::~CfgSearchBackTable()
{
    m_mSearchBack.clear();
    m_mSearchBackByType.clear();
}

void CfgSearchBackTable::Add(const CfgSearchBack& stu)
{
    if (stu.nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSearchBackTable::Add invalid id: %d", stu.nId);
        return;
    }
    
    m_mSearchBack[stu.nId] = stu;
    m_mSearchBackByType[stu.nType].push_back(stu);
}

const CfgSearchBack* CfgSearchBackTable::GetSearchBack(int32_t nId)
{
    auto it = m_mSearchBack.find(nId);
    if (it != m_mSearchBack.end())
    {
        return &it->second;
    }
    return nullptr;
}

const std::vector<CfgSearchBack>* CfgSearchBackTable::GetSearchBackByType(int32_t nType)
{
    auto it = m_mSearchBackByType.find(nType);
    if (it != m_mSearchBackByType.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgSearchBackTable::IsExist(int32_t nId)
{
    return m_mSearchBack.find(nId) != m_mSearchBack.end();
}

void CfgSearchBackTable::Clear()
{
    m_mSearchBack.clear();
    m_mSearchBackByType.clear();
}