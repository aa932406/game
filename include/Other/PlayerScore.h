#ifndef _PLAYERSCORE_H_
#define _PLAYERSCORE_H_

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

class PlayerScore
{
public:
    PlayerScore();
    ~PlayerScore();
    PlayerScore(const PlayerScore& other);

public:
    int64_t nCharId;
    std::string strName;
    int32_t nDamage;
    int32_t nIndex;
    int32_t nTeam;
    std::string teamName;
    int64_t nTeamLeaderId;
};

#endif // _PLAYERSCORE_H_
