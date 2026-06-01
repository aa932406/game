#include "Config/CfgShiZhuangTable.h"
#include "Answer/Logger.h"
#include <map>

static std::map<int32_t, CfgShiZhuang> m_mShiZhuang;
static std::map<int32_t, std::map<int32_t, CfgShiZhuangLevel>> m_mShiZhuangLevel;

CfgShiZhuangTable::CfgShiZhuangTable()
{
    m_mShiZhuang.clear();
    m_mShiZhuangLevel.clear();
}

CfgShiZhuangTable::~CfgShiZhuangTable()
{
    m_mShiZhuang.clear();
    m_mShiZhuangLevel.clear();
}

void CfgShiZhuangTable::Add(const CfgShiZhuang& stu)
{
    if (stu.nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgShiZhuangTable::Add invalid id: %d", stu.nId);
        return;
    }
    
    m_mShiZhuang[stu.nId] = stu;
}

void CfgShiZhuangTable::AddLevel(const CfgShiZhuangLevel& stu)
{
    if (stu.nType <= 0 || stu.nLevel <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgShiZhuangTable::AddLevel invalid param: type=%d, level=%d", stu.nType, stu.nLevel);
        return;
    }
    
    m_mShiZhuangLevel[stu.nType][stu.nLevel] = stu;
}

const CfgShiZhuang* CfgShiZhuangTable::GetShiZhuang(int32_t nId)
{
    auto it = m_mShiZhuang.find(nId);
    if (it != m_mShiZhuang.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgShiZhuangLevel* CfgShiZhuangTable::GetShiZhuangLevel(int32_t nType, int32_t nLevel)
{
    auto itType = m_mShiZhuangLevel.find(nType);
    if (itType != m_mShiZhuangLevel.end())
    {
        auto itLevel = itType->second.find(nLevel);
        if (itLevel != itType->second.end())
        {
            return &itLevel->second;
        }
    }
    return nullptr;
}

int32_t CfgShiZhuangTable::GetMaxLevel(int32_t nType)
{
    auto itType = m_mShiZhuangLevel.find(nType);
    if (itType != m_mShiZhuangLevel.end() && !itType->second.empty())
    {
        return itType->second.rbegin()->first;
    }
    return 0;
}

bool CfgShiZhuangTable::IsExist(int32_t nId)
{
    return m_mShiZhuang.find(nId) != m_mShiZhuang.end();
}

void CfgShiZhuangTable::Clear()
{
    m_mShiZhuang.clear();
    m_mShiZhuangLevel.clear();
}