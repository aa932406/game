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
