#ifndef _CFGSPECIALMONSTERTABLE_H_
#define _CFGSPECIALMONSTERTABLE_H_

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

struct cfgSpecialMonster;

class cfgSpecialMonsterTable
{
public:

    void Add(cfgSpecialMonster * stu);

private:
    std::map<int, cfgSpecialMonster> m_specialMonster;
};

#endif // _CFGSPECIALMONSTERTABLE_H_

