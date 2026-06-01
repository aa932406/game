#ifndef _CFGEQUIPUPPHASETABLE_H_
#define _CFGEQUIPUPPHASETABLE_H_

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
class cfgEquipUpPhase;

class cfgEquipUpPhaseTable
{
public:
    cfgEquipUpPhaseTable();

    void AddData(cfgEquipUpPhase* stu);
    const cfgEquipUpPhase* GetData(int32_t key) const;

private:
    std::list<cfgEquipUpPhase> m_cfgEquipUpPhase;
};

#endif // _CFGEQUIPUPPHASETABLE_H_
