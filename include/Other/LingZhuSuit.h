#ifndef _LINGZHUSUIT_H_
#define _LINGZHUSUIT_H_

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

class LingZhuSuit
{
public:
    LingZhuSuit();
    LingZhuSuit(const LingZhuSuit& a2);

    int32_t nSuitId;
    std::list<AddAttribute> lAttrList;
    int32_t nCount;
};

#endif // _LINGZHUSUIT_H_
