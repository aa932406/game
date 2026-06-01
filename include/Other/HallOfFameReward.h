#ifndef _HALLOFFAMEREWARD_H_
#define _HALLOFFAMEREWARD_H_

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

class HallOfFameReward
{
public:
    HallOfFameReward();
    HallOfFameReward(const HallOfFameReward& a2);

    int32_t nIndexMin;
    int32_t nIndexMax;
    int32_t nHonor;
    std::vector<MemChrBag> items;
};

#endif // _HALLOFFAMEREWARD_H_
