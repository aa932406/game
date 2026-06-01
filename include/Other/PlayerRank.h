#ifndef _PLAYERRANK_H_
#define _PLAYERRANK_H_

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

class PlayerRank
{
public:
    PlayerRank();
    PlayerRank(const PlayerRank& a2);

    int32_t nCharId;
    std::string strName;
};

#endif // _PLAYERRANK_H_
