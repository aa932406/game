#ifndef _FINDNODE_H_
#define _FINDNODE_H_

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

class FindNode
{
public:
    FindNode();
    FindNode(const FindNode& a2);

    Position parent;
};

#endif // _FINDNODE_H_
