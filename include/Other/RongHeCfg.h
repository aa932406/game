#ifndef _RONGHECFG_H_
#define _RONGHECFG_H_

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

struct RongHeItem;

class RongHeCfg
{
public:
    RongHeCfg(RongHeCfg *const this, const RongHeCfg *const a2);

    int32_t nId;
    int32_t nCostItem;
    std::list<RongHeItem> lRongHeItemList;
    int32_t nMaxRate;
    int32_t nMaxRate2;
};

#endif // _RONGHECFG_H_
