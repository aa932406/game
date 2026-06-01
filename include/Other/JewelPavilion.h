#ifndef _JEWELPAVILION_H_
#define _JEWELPAVILION_H_

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
#include "Other/ItemEffect.h"

class JewelPavilion : public ItemEffect
{
public:
    JewelPavilion();
    virtual ~JewelPavilion();
};

#endif // _JEWELPAVILION_H_
