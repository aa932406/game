#ifndef _WAN360REWARD_H_
#define _WAN360REWARD_H_

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

class Wan360Reward
{
public:
    Wan360Reward(const Wan360Reward\& a2);

    int32_t Index;
    std::vector<MemChrBag> Items;
};

#endif // _WAN360REWARD_H_