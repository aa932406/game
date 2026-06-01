#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class WorldBossInfo {
public:
    WorldBossInfo();
    void PackageBossInfo(const WorldBossInfo *const this, Answer::NetPacket *packet);
    void AddKiller(WorldBossInfo *const this, const KillerInfo *const info);
    void PackageBossInfo(const WorldBossInfo *const this, Answer::NetPacket *packet, int32_t nNowTime);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
