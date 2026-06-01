#ifndef _IMAPEVENT_H_
#define _IMAPEVENT_H_

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

class IMapEvent
{
public:
    IMapEvent();
    ~IMapEvent();

    void init(CfgMapEventList * events, int64_t nTime);
    void openEvent(int32_t nEvent, int64_t nTick);
    void closeEvent(int32_t nEvent, int64_t nTick);
    void openEvents(Int32List * events, int64_t nTick);
    void closeEvents(Int32List * events, int64_t nTick);

private:
        // TODO: 确认类型 m_events
};

#endif // _IMAPEVENT_H_