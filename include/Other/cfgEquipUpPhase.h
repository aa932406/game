#ifndef _CFGEQUIPUPPHASE_H_
#define _CFGEQUIPUPPHASE_H_

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

class cfgEquipUpPhase
{
public:
    cfgEquipUpPhase(cfgEquipUpPhase *const this, const cfgEquipUpPhase *const a2);

private:
    int32_t m_CostEquip;
    std::list<ItemData> m_CostItems;
    int32_t m_CostMoney;
    int32_t m_GiveEquip;
    int32_t m_GongGaoId;
};

#endif // _CFGEQUIPUPPHASE_H_
