#ifndef _CDROPITEMGROUP_H_
#define _CDROPITEMGROUP_H_

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

class CDropItemGroup
{
public:

    int8_t calDropType(int8_t droperType, int8_t bossSign);
    int32_t calPositionSize(int32_t nSize);
};

#endif // _CDROPITEMGROUP_H_
