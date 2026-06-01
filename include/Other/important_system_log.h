#ifndef _IMPORTANT_SYSTEM_LOG_H_
#define _IMPORTANT_SYSTEM_LOG_H_

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

class important_system_log : public platform_log_base
{
public:
    important_system_log();
    ~important_system_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t up_time;
    int32_t sys_name;
    int32_t oper_id;
    int32_t level;
};

#endif // _IMPORTANT_SYSTEM_LOG_H_
