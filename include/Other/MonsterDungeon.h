#ifndef _MONSTERDUNGEON_H_
#define _MONSTERDUNGEON_H_

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

class MonsterDungeon
{
public:
    MonsterDungeon();
    ~MonsterDungeon();

    bool refresh();
    void postDamage(int32_t damge, UnitHandle launcher, int32_t Mid);
    void reset();
    int8_t getDungeonHard();
    int8_t getDungeonQuality();
    void addEventHp(HPEvent * hpEvent);
    void die();
    void destroy();
    void remove();
    void onArriveTarget();
    void runOnRoad();

private:
        const CfgActivity* m_cfgDungeonMonster;  // TODO: 确认配置类型
        // TODO: 确认类型 m_delFlag
        // TODO: 确认类型 m_dungeon
        // TODO: 确认类型 m_eventHp
        // TODO: 确认类型 m_road
        // TODO: 确认类型 m_target
};

#endif // _MONSTERDUNGEON_H_
