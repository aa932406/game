#include "Other/ChouJiangTable.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

ChouJiangTable::ChouJiangTable()
{
}

ChouJiangTable::~ChouJiangTable()
{
}

void ChouJiangTable::AddChouJiangCfg(ChouJiangCfg stu)
{
    m_ChouJiangCfgMap[stu.Index] = stu;
}

void ChouJiangTable::AddChouJiangLuckyCfg(ChouJiangLuckyCfg stu)
{
    m_ChouJiangLuckyCfgMap[stu.Index] = stu;
}

void ChouJiangTable::AddChouJiangWeekReward(ChouJiangWeekReward *p_stu)
{
    m_ChouJiangWeekRewardMap[p_stu->Index] = *p_stu;
}

void ChouJiangTable::AddChouJiangCost(ChouJiangCost stu)
{
    m_ChouJiangCostMap[stu.Times] = stu;
}

const ChouJiangLuckyCfg* ChouJiangTable::GetChouJiangLuckyCfg(int32_t nIdex)
{
    auto it = m_ChouJiangLuckyCfgMap.find(nIdex);
    if (it != m_ChouJiangLuckyCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const ChouJiangCost* ChouJiangTable::GetChouJiangCost(int32_t nIdex)
{
    auto it = m_ChouJiangCostMap.find(nIdex);
    if (it != m_ChouJiangCostMap.end())
    {
        return &it->second;
    }
    return nullptr;
}
