#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Other/KillerInfo.h"

namespace Answer { class NetPacket; }

class WorldBossInfo {
public:
    WorldBossInfo();
    void PackageBossInfo(Answer::NetPacket *packet) const;
    void AddKiller(const KillerInfo *const info);
    void PackageBossInfo(Answer::NetPacket *packet, int32_t nNowTime) const;

    int32_t nBossId;
    int32_t nLevel;
    int32_t nExp;
    int32_t nMid;
    int32_t nBossType;
    int32_t nMapId;
    int32_t nReviveTime;
    int8_t nState;
    KillerInfo vKiller[5];
    void* pPlant;
};
