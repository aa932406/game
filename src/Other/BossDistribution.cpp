#include "Other/BossDistribution.h"

#include <cstring>
#include <algorithm>

BossDistribution::BossDistribution()
{
}

BossDistribution::~BossDistribution()
{
}

void BossDistribution::AddBossLevelInfo(BossLevelInfo *p_stu)
{
    m_BossLevelInfoMap[p_stu->BossLevel] = *p_stu;
}

void BossDistribution::AddMapBossInfo(MapBossInfo *p_stu)
{
    m_MapBossInfoMap[p_stu->nId] = *p_stu;
}

Int32List BossDistribution::GetBossLevelInfo(int32_t Level)
{
    Int32List result;
    auto it = m_BossLevelInfoMap.find(Level);
    if (it != m_BossLevelInfoMap.end())
    {
        result = it->second.BossMapList;
    }
    return result;
}

Int32List BossDistribution::GetMapBossInfo(int32_t id)
{
    Int32List result;
    auto it = m_MapBossInfoMap.find(id);
    if (it != m_MapBossInfoMap.end())
    {
        result = it->second.BossMapList;
    }
    return result;
}
