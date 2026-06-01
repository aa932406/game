#ifndef _VPLANREWARD_H_
#define _VPLANREWARD_H_

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

class VplanReward
{
public:
    VplanReward();
    ~VplanReward();
    VplanReward(const VplanReward& other);

private:
    int32_t Index;
    std::vector<MemChrBag> Reward;
    std::vector<MemChrBag> YearVipReward;
};

#endif // _VPLANREWARD_H_
