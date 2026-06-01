#ifndef _GAMEMSG_H_
#define _GAMEMSG_H_

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

enum class GameMsgCode : int32_t
{
    GMC_UNKNOWN            = 0,
    GMC_PLAYER_ENTER_MAP   = 1,
    GMC_PLAYER_LEAVE_MAP   = 2,
    GMC_ADD_DUNGEON        = 3,
    GMC_DEL_DUNGEON        = 4,
    GMC_PLAYER_LOGOUT      = 5,
    GMC_ACTIVITY_START     = 6,
    GMC_CITY_STAR          = 7,
    GMC_GOBLIN_ACTIVITY    = 8,
    GMC_ACTIVITY_STOP      = 9,
    GMC_ADD_TERRITORY      = 10,
    GMC_DEL_TERRITORY      = 11,
    GMC_ADD_WORLD_BOSS_NPC = 12,
    GMC_ADD_OBJ            = 13,
};

class GameMsg
{
public:
    void reset();

    GameMsgCode msgcode;
    void* ptr1;
    void* ptr2;
    void* ptr3;
    int32_t iparam1;
    int32_t iparam2;
    int32_t effecttick;
};

#endif // _GAMEMSG_H_
