#include "Other/BossLevelInfo.h"

BossLevelInfo::BossLevelInfo()
    : BossLevel(0)
{
}

BossLevelInfo::BossLevelInfo(const BossLevelInfo& a2)
    : BossLevel(a2.BossLevel)
    , BossMapList(a2.BossMapList)
{
}
