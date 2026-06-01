#ifndef _WINGEQUIPPOLISH_H_
#define _WINGEQUIPPOLISH_H_

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

class WingEquipPolish
{
public:
    WingEquipPolish();
    ~WingEquipPolish();
    WingEquipPolish(const WingEquipPolish& other);

    int32_t nSlot;
    int32_t nLevel;
    std::list<AddAttribute> lAttrList;
    std::list<ItemData> lCostList;
    int32_t nConstMoney;
    int32_t SuitId;
    int32_t GongGaoId;
};

#endif // _WINGEQUIPPOLISH_H_