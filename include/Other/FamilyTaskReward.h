#ifndef _FAMILYTASKREWARD_H_
#define _FAMILYTASKREWARD_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Common/CommonTypes.h"

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

class FamilyTaskReward
{
public:
    FamilyTaskReward();
    FamilyTaskReward(const FamilyTaskReward& a2);

    int32_t Index;
    int32_t NeedCount;
    std::vector<MemChrBag> Rewards;
};

#endif // _FAMILYTASKREWARD_H_
