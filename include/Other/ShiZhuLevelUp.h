#ifndef _SHIZHULEVELUP_H_
#define _SHIZHULEVELUP_H_

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
struct AddAttribute;

class ShiZhuLevelUp
{
public:
    ShiZhuLevelUp();
    ~ShiZhuLevelUp();
    ShiZhuLevelUp(const ShiZhuLevelUp& other);

private:
    int32_t nId;
    int32_t nLevel;
    std::list<ItemData> lCostItem;
    std::list<AddAttribute> lAttr;
};

#endif // _SHIZHULEVELUP_H_
