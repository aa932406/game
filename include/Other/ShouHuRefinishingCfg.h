#ifndef _SHOUHUREFINISHINGCFG_H_
#define _SHOUHUREFINISHINGCFG_H_

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

struct AddAttribute;
struct ItemData;

class ShouHuRefinishingCfg
{
public:
    ~ShouHuRefinishingCfg();

    std::list<AddAttribute> lAttrList;
    std::list<ItemData> lCostList;
};

#endif // _SHOUHUREFINISHINGCFG_H_
