#ifndef _LIANRECHARGECFG_H_
#define _LIANRECHARGECFG_H_

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

class LianRechargeCfg
{
public:
    LianRechargeCfg();
    LianRechargeCfg(const LianRechargeCfg& a2);

    int32_t nValues;
    std::vector<std::vector<MemChrBag>> vGiftVector;
};

#endif // _LIANRECHARGECFG_H_
