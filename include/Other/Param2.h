#ifndef _PARAM2_H_
#define _PARAM2_H_

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

class Param2
{
public:
    Param2();
    Param2(int32_t param1, int32_t param2);

    int32_t nParam1;
    int32_t nParam2;
};

#endif // _PARAM2_H_
