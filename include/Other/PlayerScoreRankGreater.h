#ifndef _PLAYERSCORERANKGREATER_H_
#define _PLAYERSCORERANKGREATER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Common/CommonTypes.h"

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

class PlayerScoreRankGreater
{
public:
    bool operator()(const PlayerScore* _Left, const PlayerScore* _Right);
};

#endif // _PLAYERSCORERANKGREATER_H_
