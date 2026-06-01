#ifndef _GROUPMONSTER_H_
#define _GROUPMONSTER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Utility/Position.h"

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

class GroupMonster
{
public:
    GroupMonster();
    GroupMonster(const GroupMonster& a2);

    int32_t RegionCount;
    int32_t Mid;
    int32_t MonsterCount;
    int32_t IsSpecial;
    std::vector<Position> RevivePos;
};

#endif // _GROUPMONSTER_H_
