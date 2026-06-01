#ifndef _SKILL_LOG_H_
#define _SKILL_LOG_H_

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

class skill_log : public platform_log_base
{
public:
    skill_log();
    ~skill_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t op_time;
    int32_t skill_id;
    int32_t oper_id;
    int32_t level;
};

#endif // _SKILL_LOG_H_
