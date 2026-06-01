#ifndef _VIPEQUIPPOSLEVELUP_H_
#define _VIPEQUIPPOSLEVELUP_H_

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

class VipEquipPosLevelUp
{
public:
    VipEquipPosLevelUp();
    ~VipEquipPosLevelUp();
    VipEquipPosLevelUp(const VipEquipPosLevelUp& other);

    int32_t nSlot;
    int32_t nLevel;
    int32_t nRate;
    std::list<AddAttribute> lAddAttrs;
    std::list<ItemData> lCostItems;
    int32_t nGongGaoId;
};

#endif // _VIPEQUIPPOSLEVELUP_H_