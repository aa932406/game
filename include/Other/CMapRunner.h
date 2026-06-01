#ifndef _CMAPRUNNER_H_
#define _CMAPRUNNER_H_

// 如果 Map/CMapRunner.h 已定义，则跳过本文件
#ifndef CMAPRUNNER_CLASS_DEFINED
#define CMAPRUNNER_CLASS_DEFINED

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
class Dungeon;
class CTerritory;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class CMapRunner
{
public:
    CMapRunner();
    ~CMapRunner();

    void run();
    void getLocalNow(tm &retstr);
    void Stop();
    void AddMap(Map *pMap);
    void DelMap(MapId_t nMapId);
    void update();
    void checkMessage();
    void dealMessage(GameMsg *msg);
    void onPlayerEnterMap(Player *pPlayer, Map *pMap, int32_t x, int32_t y);
    void onPlayerLeaveMap(Player *pPlayer, Map *pMap, int32_t x, int32_t y);
    void onAddDungeon(Dungeon *pDungeon);
    void onDelDungeon(Dungeon *pDungeon);
    void onPlayerLogout(Player *pPlayer);
    void onAddTerritory(CTerritory *pTerritory);
    void onDelTerritory(CTerritory *pTerritory);
    void onActivityStart(CActivityMap *pMap, CActivity *pActivity);
    void onActivityStop(CActivityMap *pMap, CActivity *pActivity);
    void onStarCityWar(Map *pMap);

private:
        // TODO: 确认类型 m_bRunning
        // TODO: 确认类型 m_maps
        // TODO: 确认类型 m_msgQueue
        // TODO: 确认类型 m_tmLocalNow
};

#endif // CMAPRUNNER_CLASS_DEFINED
#endif // _CMAPRUNNER_H_
