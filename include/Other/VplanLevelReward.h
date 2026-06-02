#ifndef _VPLANLEVELREWARD_H_
#define _VPLANLEVELREWARD_H_

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

struct MemChrBag;

class VplanLevelReward
{
public:
    VplanLevelReward(const VplanLevelReward& a2);

    int32_t Index;
    int32_t Level;
    std::vector<MemChrBag> Reward;
};

#endif // _VPLANLEVELREWARD_H_