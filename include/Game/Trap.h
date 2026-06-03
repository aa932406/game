#ifndef _TRAP_H_
#define _TRAP_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

enum class TrapState : int32_t
{
    TS_NONE = 0,
    TS_ACTIVE = 1,
    TS_TRIGGERED = 2,
    TS_COOLDOWN = 3
};

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

class Trap
{
public:
    Trap();
    ~Trap();

    void init(Map *pMap, Position pos, CfgTrap * cfgTrap);
    void reset();
    void checkState();
    int32_t onUse(Player *player);
    bool appendInfo(Answer::NetPacket *packet);
    void broadcastState();
    void broadcastLeaveMap();
    void setState(TrapState state);
    void effect();

private:
        const CfgActivity* m_cfgTrap;
        Position m_currentPos;
        Position m_currentTile;
        int64_t m_liftTime;
        Map* m_pMap;
        TrapState m_state;
        int64_t m_stateTick;
        Player* m_user;
};

#endif // _TRAP_H_