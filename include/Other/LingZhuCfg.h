#ifndef _LINGZHUCFG_H_
#define _LINGZHUCFG_H_

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

class LingZhuCfg
{
public:
    LingZhuCfg();
    LingZhuCfg(const LingZhuCfg& a2);

    int32_t nId;
    int32_t nType;
    int32_t nLevel;
    std::list<AddAttribute> lAttrList;
    int32_t nKunLingLevel;
    int32_t nQuality;
    int32_t nDecExp;
};

#endif // _LINGZHUCFG_H_
