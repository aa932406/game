// Write implementation for BossDistribution
$cpp = @"
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
"@

Set-Content "D:\444\gameserver_src\src\Other\BossDistribution.cpp" $cpp

# Fix header member types
$h = @"
#ifndef _BOSSDISTRIBUTION_H_
#define _BOSSDISTRIBUTION_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Other/BossLevelInfo.h"
#include "Other/MapBossInfo.h"

class BossDistribution
{
public:
    BossDistribution();
    ~BossDistribution();

    void AddBossLevelInfo(BossLevelInfo *p_stu);
    void AddMapBossInfo(MapBossInfo *p_stu);
    Int32List GetBossLevelInfo(int32_t Level);
    Int32List GetMapBossInfo(int32_t id);

private:
    std::map<int32_t, BossLevelInfo> m_BossLevelInfoMap;
    std::map<int32_t, MapBossInfo> m_MapBossInfoMap;
};

#endif // _BOSSDISTRIBUTION_H_
"@

Set-Content "D:\444\gameserver_src\include\Other\BossDistribution.h" $h

Write-Output "BossDistribution implemented"
