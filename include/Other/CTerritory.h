#ifndef _CTERRITORY_H_
#define _CTERRITORY_H_

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

class CTerritory
{
public:
    CTerritory();
    ~CTerritory();

    void removePlayer(Player *player, bool islogout);
    void reset();
    void AddFamilyMonster(bool bInit);
    void addPlayer(Player *player, int32_t x, int32_t y);
    void addMonster(Monster *monster, int32_t x, int32_t y);
    void onMonsterDie(Monster *monster, Player *player, bool IsGroupMonster);

private:
        int32_t m_FamilyId;
        // TODO: 确认类型 m_Piglet
        // TODO: 确认类型 m_dropItems
        // TODO: 确认类型 m_monsters
        // TODO: 确认类型 m_npcs
        // TODO: 确认类型 m_plants
        // TODO: 确认类型 m_players
        // TODO: 确认类型 m_traps
};

#endif // _CTERRITORY_H_
