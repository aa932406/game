#ifndef _LEVEL_UP_LOG_H_
#define _LEVEL_UP_LOG_H_

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

class level_up_log : public platform_log_base
{
public:
    level_up_log();
    ~level_up_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t create_time;
    int32_t level_up_time;
    int32_t level;
    int32_t Level_interval;
};

#endif // _LEVEL_UP_LOG_H_
