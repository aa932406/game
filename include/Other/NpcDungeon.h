#ifndef _NPCDUNGEON_H_
#define _NPCDUNGEON_H_

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

class NpcDungeon
{
public:
    NpcDungeon();
    ~NpcDungeon();

    void init(Dungeon *dungeon, CfgNpc * cfgNpc);
    int32_t onEndNpc(Player *player, int8_t choice);

private:
        // TODO: 确认类型 m_dungeon
};

#endif // _NPCDUNGEON_H_
