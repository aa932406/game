#ifndef _VPLANEVERYDAYREWARD_H_
#define _VPLANEVERYDAYREWARD_H_

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

class VplanEveryDayReward
{
public:
    VplanEveryDayReward();
    ~VplanEveryDayReward();
    VplanEveryDayReward(const VplanEveryDayReward& other);

    std::vector<MemChrBag> Reward;
    std::vector<MemChrBag> YearVipReward;
};

#endif // _VPLANEVERYDAYREWARD_H_