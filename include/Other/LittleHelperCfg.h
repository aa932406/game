#ifndef _LITTLEHELPERCFG_H_
#define _LITTLEHELPERCFG_H_

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

#ifndef LITTLE_HELPER_CFG_DEFINED
#define LITTLE_HELPER_CFG_DEFINED

class LittleHelperCfg
{
public:
    LittleHelperCfg();
    LittleHelperCfg(const LittleHelperCfg& a2);

    int32_t nId;
    int32_t nSpeed;
    std::list<AddAttribute> lAttr;
    int32_t nGold;
    int32_t nReGold;
    int32_t nTime;
    int32_t nGongGaoId;
};

#endif // LITTLE_HELPER_CFG_DEFINED
#endif // _LITTLEHELPERCFG_H_
