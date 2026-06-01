#ifndef _MAIN_TASK_LOG_H_
#define _MAIN_TASK_LOG_H_

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

class main_task_log : public platform_log_base
{
public:
    main_task_log();
    ~main_task_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t op_time;
    int32_t task_id;
    int32_t oper_id;
};

#endif // _MAIN_TASK_LOG_H_
