#ifndef _TREASUREHUNTERCFG_H_
#define _TREASUREHUNTERCFG_H_

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

class TreasureHunterCfg
{
public:
    TreasureHunterCfg(TreasureHunterCfg *const this, const TreasureHunterCfg *const a2);

    int32_t nId;
    std::vector<MemChrBag> Items;
    int32_t nRequire;
    int32_t GongGaoId;
    int32_t nType;
};

#endif // _TREASUREHUNTERCFG_H_