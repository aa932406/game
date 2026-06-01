#ifndef _PLANTDUNGEON_H_
#define _PLANTDUNGEON_H_

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

class PlantDungeon
{
public:
    PlantDungeon();
    ~PlantDungeon();

    int32_t onEndGather(Player *player);
    int32_t onBeginGather(Player *player);

private:
        // TODO: 确认类型 m_dungeon
};

#endif // _PLANTDUNGEON_H_
