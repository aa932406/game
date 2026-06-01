#ifndef _WORLDBOSSINFO_H_
#define _WORLDBOSSINFO_H_

#include <cstdint>
#include <string>

#include "Other/KillerInfo.h"

namespace Answer { class NetPacket; }

class WorldBossInfo
{
public:
    WorldBossInfo();
    WorldBossInfo(const WorldBossInfo& other);
    ~WorldBossInfo();

    WorldBossInfo& operator=(const WorldBossInfo& other);

    void PackageBossInfo(Answer::NetPacket* packet);
    void UnPackageBossInfo(Answer::NetPacket* inPacket);
    void AddKiller(const KillerInfo* info);

    int32_t nBossId;
    int32_t nLevel;
    int32_t nExp;
    int32_t nMid;
    int32_t nBossType;
    int32_t nMapId;
    int32_t nReviveTime;
    int8_t nState;
    void* pPlant;
    KillerInfo vKiller[5];
};

#endif // _WORLDBOSSINFO_H_
