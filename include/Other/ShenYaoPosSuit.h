#ifndef _SHENYAOPOSSUIT_H_
#define _SHENYAOPOSSUIT_H_

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

struct AttrAddon;

class ShenYaoPosSuit
{
public:
    ShenYaoPosSuit(const ShenYaoPosSuit\& a2);

    int32_t nLevel;
    std::vector<AttrAddon> vAttrAddon;
};

#endif // _SHENYAOPOSSUIT_H_