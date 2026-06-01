#ifndef _LUDASHIVIP_H_
#define _LUDASHIVIP_H_

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

class LuDaShiVip
{
public:
    LuDaShiVip();
    LuDaShiVip(const LuDaShiVip& a2);

    int32_t nIndex;
    int32_t nType;
    std::vector<MemChrBag> Rewards;
    int32_t nCondition;
    int32_t nMaxCondition;
    int32_t nVipType;
};

#endif // _LUDASHIVIP_H_
