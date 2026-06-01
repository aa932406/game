#ifndef _CURRENCY_LOG_H_
#define _CURRENCY_LOG_H_

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

class currency_log : public platform_log_base
{
public:
    currency_log();
    ~currency_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t time;
    int8_t type;
    int32_t opway;
    int64_t value;
    std::string ids;
    std::string couts;
    int32_t level;
};

#endif // _CURRENCY_LOG_H_
