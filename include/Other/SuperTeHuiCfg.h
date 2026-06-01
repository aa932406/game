#ifndef _SUPERTEHUICFG_H_
#define _SUPERTEHUICFG_H_

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

class SuperTeHuiCfg
{
public:
    SuperTeHuiCfg(SuperTeHuiCfg *const this, const SuperTeHuiCfg *const a2);

    int32_t nIndex;
    int32_t nNeedVip;
    std::vector<MemChrBag> Items;
    int32_t nPrice;
    int32_t nGongGaoId;
};

#endif // _SUPERTEHUICFG_H_