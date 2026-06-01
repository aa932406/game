#ifndef _CMINGGECFG_H_
#define _CMINGGECFG_H_

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

class CMingGeCfg
{
public:
    CMingGeCfg();
    CMingGeCfg(const CMingGeCfg& a2);

    int32_t nId;
    std::list<AddAttribute> lAttrList;
    int32_t nType;
    int32_t nCanPickup;
    int32_t nLevel;
    int32_t nQuality;
    int32_t nDecExp;
};

#endif // _CMINGGECFG_H_
