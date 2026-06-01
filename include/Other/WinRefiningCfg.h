#ifndef _WINREFININGCFG_H_
#define _WINREFININGCFG_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Config/CfgData.h"

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

struct ItemData;

class WinRefiningCfg
{
public:
    WinRefiningCfg(WinRefiningCfg *const this, const WinRefiningCfg *const a2);

    int32_t nSlot;
    int32_t nLevel;
    AddAttribute Attr;
    std::list<ItemData> lCostList;
    int32_t SuitId;
    int32_t nConstMoney;
    int32_t GongGaoId;
};

#endif // _WINREFININGCFG_H_