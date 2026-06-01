#ifndef _ITEM_LOG_H_
#define _ITEM_LOG_H_

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

class item_log : public platform_log_base
{
public:
    item_log();
    ~item_log();

    void CleanUp() override;
    void PackageData(Answer::NetPacket *packet) override;
    void UnPackageData(Answer::NetPacket *inPacket) override;

    std::string passport;
    int64_t cid;
    std::string name;
    int32_t op_time;
    std::string ItemId;
    int32_t flag;
    int32_t opway;
    int32_t count;
    int32_t level;
};

#endif // _ITEM_LOG_H_
