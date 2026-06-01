#ifndef _BOSSKILLEDREWARDINFO_H_
#define _BOSSKILLEDREWARDINFO_H_

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

class BossKilledRewardInfo
{
public:

    bool CanGetReward();
    bool SetBossKilled(int32_t BossId);

private:
    int32_t IsGetReward;
    std::list<int32_t> BossKeilled;
};

#endif // _BOSSKILLEDREWARDINFO_H_
