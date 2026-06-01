#include "Other/CMingGeTable.h"
#include "Other/CMingGeCfg.h"
#include "Other/Param2.h"
#include "Config/CfgData.h"

#include <cstring>
#include <cstdlib>
#include <algorithm>

CMingGeTable::CMingGeTable()
{
}

CMingGeTable::~CMingGeTable()
{
}

const CMingGeCfg* CMingGeTable::GetCMingGeCfg(int32_t nId)
{
    auto it = m_CMingGeCfgMap.find(nId);
    if (it != m_CMingGeCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CMingGeTable::AddMingGeCfg(CMingGeCfg *p_stu)
{
    m_CMingGeCfgMap[p_stu->nId] = *p_stu;
}

void CMingGeTable::AddMingGeDrawCost(MingGeDrawCost stu)
{
    m_MingGeDrawCostMap[stu.nType] = stu;
}

void CMingGeTable::AddMingGeDrawRate(int32_t nType, Param2 ItemRate)
{
    MingGeDrawRate rate;
    rate.nType = nType;
    rate.nParam1 = ItemRate.nParam1;
    rate.nParam2 = ItemRate.nParam2;
    m_MingGeDrawRateMap[nType] = rate;
}

const MingGeDrawCost* CMingGeTable::GetMingGeDrawCost(int32_t nType)
{
    auto it = m_MingGeDrawCostMap.find(nType);
    if (it != m_MingGeDrawCostMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CMingGeTable::RandMingGe(int32_t nType)
{
    auto it = m_MingGeDrawRateMap.find(nType);
    if (it == m_MingGeDrawRateMap.end())
    {
        return 0;
    }
    const MingGeDrawRate& rate = it->second;
    if (rate.nMaxRate <= 0)
    {
        return 0;
    }
    int32_t nRand = rand() % rate.nMaxRate;
    int32_t nCumulative = 0;
    for (auto it2 = m_MingGeDrawRateMap.begin(); it2 != m_MingGeDrawRateMap.end(); ++it2)
    {
        nCumulative += it2->second.nParam2;
        if (nRand < nCumulative)
        {
            return it2->second.nParam1;
        }
    }
    return 0;
}
