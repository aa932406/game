#ifndef _SUMMONSKILL_H_
#define _SUMMONSKILL_H_

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

class SummonSkill
{
public:
    SummonSkill(SummonSkill *const this);

    Position targetPos;
};

#endif // _SUMMONSKILL_H_
