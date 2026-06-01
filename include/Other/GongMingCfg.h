#ifndef _GONGMINGCFG_H_
#define _GONGMINGCFG_H_

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

class GongMingCfg
{
public:
    GongMingCfg();
    GongMingCfg(const GongMingCfg& a2);

    int32_t nChong;
    int32_t nStar;
    int32_t nIndex;
    int32_t NeedGongMingZhi;
    std::list<AddAttribute> lAttrList;
    int32_t GongGaoId;
};

#endif // _GONGMINGCFG_H_
