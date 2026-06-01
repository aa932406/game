#ifndef _KUNLEVELUPCFG_H_
#define _KUNLEVELUPCFG_H_

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

class KunLevelUpCfg
{
public:
    KunLevelUpCfg();
    KunLevelUpCfg(const KunLevelUpCfg& a2);

    int32_t nLevel;
    int32_t nExp;
    std::list<AddAttribute> lAttrList;
    int32_t nGongGaoId;
    int32_t nTalentId;
    int32_t nTalentLevel;
    int32_t nCostMoney;
};

#endif // _KUNLEVELUPCFG_H_
