#ifndef _BOSSKILLEDREWARD_H_
#define _BOSSKILLEDREWARD_H_

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

class BossKilledReward
{
public:
    BossKilledReward();
    ~BossKilledReward();
    BossKilledReward(const BossKilledReward& other);

private:
    int32_t nType;
    std::list<int> BossList;
    std::vector<MemChrBag> Rewars;
};

#endif // _BOSSKILLEDREWARD_H_
