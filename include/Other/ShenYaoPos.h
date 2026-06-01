#ifndef _SHENYAOPOS_H_
#define _SHENYAOPOS_H_

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
struct ItemData;
struct AttrAddon;

class ShenYaoPos
{
public:
    ShenYaoPos();
    ~ShenYaoPos();
    ShenYaoPos(const ShenYaoPos& other);

private:
    int32_t nSlotId;
    int32_t nLevel;
    std::vector<AttrAddon> vAttrAddon;
    std::list<ItemData> nCost;
    int32_t nGongGaoId;
};

#endif // _SHENYAOPOS_H_
