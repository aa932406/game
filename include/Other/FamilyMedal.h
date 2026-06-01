#ifndef _FAMILYMEDAL_H_
#define _FAMILYMEDAL_H_

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

class FamilyMedal
{
public:
    FamilyMedal();
    FamilyMedal(const FamilyMedal& a2);

    int32_t nLevel;
    int32_t nNeedFamilyLevel;
    int32_t nNeedFamilyExp;
    std::list<AddAttribute> lAttrList;
    int32_t nGongGaoId;
    int32_t DefRdc;
};

#endif // _FAMILYMEDAL_H_
