#ifndef _NODE_H_
#define _NODE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Utility/Position.h"

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

class Node
{
public:
    Node();
    Node(const Position& position);

    Position pos;
    int32_t inOpen;
    int32_t inClose;
    int32_t g;
    int32_t h;
    int32_t f;
};

#endif // _NODE_H_
