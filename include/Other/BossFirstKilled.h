#ifndef _BOSSFIRSTKILLED_H_
#define _BOSSFIRSTKILLED_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

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

class BossFirstKilled
{
public:
    BossFirstKilled();
    BossFirstKilled(const BossFirstKilled& a2);

    int32_t BossId;
    int32_t RewardType;
    int32_t RewardValue;
    int32_t Killer;
    std::string KillerName;
    int32_t KillTime;
    int32_t GongGaoId;
};

#endif // _BOSSFIRSTKILLED_H_
