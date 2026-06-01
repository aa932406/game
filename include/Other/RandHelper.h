#ifndef _RANDHELPER_H_
#define _RANDHELPER_H_

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

class RandHelper
{
public:
    static int32_t CalRandTimes(int32_t nTotalTimes, int32_t nRate, int32_t nTotalRate);

};

#endif // _RANDHELPER_H_
