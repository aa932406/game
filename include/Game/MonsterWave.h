#ifndef _MONSTERWAVE_H_
#define _MONSTERWAVE_H_

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

class MonsterWave
{
public:

    bool alive();
};

#endif // _MONSTERWAVE_H_