#ifndef _DEATH_LOG_H_
#define _DEATH_LOG_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Other/platform_log_base.h"

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class death_log : public platform_log_base
{
public:
    death_log();
    ~death_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t death_type;
    int32_t death_time;
    int32_t mapId;
    int32_t level;
    int32_t kill_id;
    int32_t x;
    int32_t y;
};

#endif // _DEATH_LOG_H_
