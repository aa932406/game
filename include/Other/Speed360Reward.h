#ifndef _SPEED360REWARD_H_
#define _SPEED360REWARD_H_

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

class Speed360Reward
{
public:
    Speed360Reward(Speed360Reward *const this, const Speed360Reward *const a2);

    int32_t StartTime;
    int32_t EndTime;
    std::vector<MemChrBag> Rewards;
};

#endif // _SPEED360REWARD_H_