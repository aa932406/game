#ifndef _FAMILYSELFMEDAL_H_
#define _FAMILYSELFMEDAL_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Config/CfgData.h"

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

class FamilySelfMedal
{
public:
    FamilySelfMedal();
    FamilySelfMedal(const FamilySelfMedal& a2);

    int32_t nLevel;
    int32_t nNeedExp;
    int32_t nAttrRate;
    std::list<AddAttribute> lAttrList;
};

#endif // _FAMILYSELFMEDAL_H_
