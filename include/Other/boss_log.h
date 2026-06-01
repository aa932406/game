#ifndef _BOSS_LOG_H_
#define _BOSS_LOG_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Other/platform_log_base.h"
#include <string>

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

class boss_log : public platform_log_base
{
public:
    boss_log();
    ~boss_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    std::string name;
    int64_t cid;
    int32_t kill_time;
    int32_t mid;
    int32_t boss_id;
};

#endif // _BOSS_LOG_H_
