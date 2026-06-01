#ifndef _EQUIPJINHUA_H_
#define _EQUIPJINHUA_H_

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
struct Param2;

class EquipJinHua
{
public:
    EquipJinHua();
    ~EquipJinHua();
    EquipJinHua(const EquipJinHua& other);

    int32_t GetParamRate();

    int32_t EquipId;
    std::list<ItemData> CostItems;
    std::list<Param2> Rate;
    int32_t MaxRate;
};

#endif // _EQUIPJINHUA_H_