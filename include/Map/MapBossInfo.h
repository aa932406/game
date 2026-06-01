#pragma once
#include <cstdint>

class MapBossInfo
{
public:
    MapBossInfo();
    MapBossInfo &operator=(const MapBossInfo &a2);

    int32_t m_bossId;
    int32_t m_mapId;
    int32_t m_bossLevel;
};
