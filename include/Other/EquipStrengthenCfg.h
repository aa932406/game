#ifndef _EQUIPSTRENGTHENCFG_H_
#define _EQUIPSTRENGTHENCFG_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Common/CommonTypes.h"

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

class EquipStrengthenCfg
{
public:
    EquipStrengthenCfg();
    EquipStrengthenCfg(const EquipStrengthenCfg& a2);

    int32_t Level;
    int32_t Pos;
    int32_t ConstItem;
    int32_t AttrRate;
    int32_t DropRate;
    std::vector<AttrAddon> vAttrAddon;
    int32_t NeedPos;
};

#endif // _EQUIPSTRENGTHENCFG_H_
