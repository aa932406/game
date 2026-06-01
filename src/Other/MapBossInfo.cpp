#include "Other/MapBossInfo.h"

MapBossInfo::MapBossInfo()
    : nId(0)
{
}

MapBossInfo::MapBossInfo(const MapBossInfo& a2)
    : nId(a2.nId)
    , BossMapList(a2.BossMapList)
{
}
