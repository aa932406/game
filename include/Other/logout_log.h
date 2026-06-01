#ifndef _LOGOUT_LOG_H_
#define _LOGOUT_LOG_H_

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

class logout_log : public platform_log_base
{
public:
    logout_log();
    ~logout_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t create_time;
    int32_t logout_time;
    int32_t online_time;
    std::string ip;
    int8_t job;
    int32_t level;
    int64_t battle;
    int32_t huoyuedu;
    int32_t map_id;
    int32_t task_id;
    int64_t recharge_sum;
    int64_t gold;
    int64_t cash;
    int64_t money;
    int64_t bind_money;
};

#endif // _LOGOUT_LOG_H_
