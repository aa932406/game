#ifndef _LUCKDROP_H_
#define _LUCKDROP_H_

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

struct SpecialItemDrop
{
    struct
    {
        int32_t itemId;
        int32_t itemClass;
        int32_t itemCount;
        int32_t bind;
    } Item;
    int32_t Rate;
};

class LuckDrop
{
public:
    LuckDrop();
    LuckDrop(const LuckDrop& a2);

    int32_t Type;
    int32_t VipLevel;
    std::list<SpecialItemDrop> ItemList;
    int32_t Rate;
};

#endif // _LUCKDROP_H_
