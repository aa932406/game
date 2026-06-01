#ifndef _CVIPCLUBLUCKDROP_H_
#define _CVIPCLUBLUCKDROP_H_

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

struct RateItem
{
    int32_t nItemId;
    int32_t nRate;
};

class CVipClubLuckyDrop
{
public:
    CVipClubLuckyDrop();
    CVipClubLuckyDrop(const CVipClubLuckyDrop& a2);

    int32_t nCondition;
    int32_t nLevel;
    std::list<RateItem> lRateItemList;
    int32_t nRand;
};

#endif // _CVIPCLUBLUCKDROP_H_
