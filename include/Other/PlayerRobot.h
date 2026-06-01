#ifndef _PLAYERROBOT_H_
#define _PLAYERROBOT_H_

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

class PlayerRobot : public Player
{
public:
    PlayerRobot();
    virtual ~PlayerRobot();
};

#endif // _PLAYERROBOT_H_
