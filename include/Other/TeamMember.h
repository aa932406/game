#ifndef _TEAMMEMBER_H_
#define _TEAMMEMBER_H_

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

class TeamMember
{
public:
    TeamMember();
    TeamMember(CharId_t cid, const std::string& name, int16_t idx);
    TeamMember(const TeamMember& other);

    CharId_t nCharId;
    std::string strName;
    int16_t nIndex;
};

#endif // _TEAMMEMBER_H_