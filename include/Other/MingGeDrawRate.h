#ifndef _MINGGEDRAWRATE_H_
#define _MINGGEDRAWRATE_H_

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

class MingGeDrawRate
{
public:
    MingGeDrawRate();
    MingGeDrawRate(const MingGeDrawRate& other);

    std::list<Param2> nItemRate;
    int32_t nMaxRate;
};

#endif // _MINGGEDRAWRATE_H_