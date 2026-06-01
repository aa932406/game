#ifndef _PATHFINDER_H_
#define _PATHFINDER_H_

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

class PathFinder
{
public:

    int32_t gValue(Node *pSrc, Position dest);
    int32_t hValue(Position src, Position dest);
};

#endif // _PATHFINDER_H_
