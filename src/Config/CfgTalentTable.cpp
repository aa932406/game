#include "Config/CfgTalentTable.h"
#include "Answer/Logger.h"
#include <map>
#include <vector>

static std::map<int32_t, std::map<int32_t, CfgTalent>> m_mTalent;  // [id][level]
static std::map<int32_t, CfgTalentPage> m_mTalentPage;  // [job]

CfgTalentTable::CfgTalentTable()
{
    m_mTalent.clear();
    m_mTalentPage.clear();
}

CfgTalentTable::~CfgTalentTable()
{
    m_mTalent.clear();
    m_mTalentPage.clear();
}

void CfgTalentTable::AddTalent(const CfgTalent& stu)
{
    if (stu.id <= 0 || stu.Level <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTalentTable::AddTalent invalid param: id=%d, level=%d", stu.id, stu.Level);
        return;
    }
    
    m_mTalent[stu.id][stu.Level] = stu;
}

void CfgTalentTable::AddTalentPage(const CfgTalentPage& stu)
{
    if (stu.job < 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTalentTable::AddTalentPage invalid job: %d", stu.job);
        return;
    }
    
    m_mTalentPage[stu.job] = stu;
}

const CfgTalent* CfgTalentTable::GetTalent(int32_t id, int32_t level)
{
    auto itId = m_mTalent.find(id);
    if (itId != m_mTalent.end())
    {
        auto itLevel = itId->second.find(level);
        if (itLevel != itId->second.end())
        {
            return &itLevel->second;
        }
    }
    return nullptr;
}

int32_t CfgTalentTable::GetTalentMaxLevel(int32_t id)
{
    auto itId = m_mTalent.find(id);
    if (itId != m_mTalent.end() && !itId->second.empty())
    {
        return itId->second.rbegin()->first;
    }
    return 0;
}

const CfgTalentPage* CfgTalentTable::GetTalentPage(int32_t job)
{
    auto it = m_mTalentPage.find(job);
    if (it != m_mTalentPage.end())
    {
        return &it->second;
    }
    return nullptr;
}

std::vector<int32_t> CfgTalentTable::GetAllTalentIds()
{
    std::vector<int32_t> result;
    for (const auto& pair : m_mTalent)
    {
        result.push_back(pair.first);
    }
    return result;
}

bool CfgTalentTable::IsTalentExist(int32_t id)
{
    return m_mTalent.find(id) != m_mTalent.end();
}

void CfgTalentTable::Clear()
{
    m_mTalent.clear();
    m_mTalentPage.clear();
}