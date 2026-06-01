#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Common/CommonTypes.h"
#include "Other/GameMsg.h"

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
class Dungeon;
class CTerritory;

class GameMsg;

class MapManager
{
public:
    MapManager();
    ~MapManager();

    void StartAll();
    void StopAll();
    void initRunner();
    void initMap();
    void AddDungeon(Dungeon *pDungeon);
    void NewDungeon(int32_t nDungenId);
    void DelDungeon(int32_t nId);
    int32_t newDungenId();
    void GetDungeon(int32_t nId);
    void GetTerritory(FamilyId_t FamilyID);
    void NewTerritory(FamilyId_t FamilyID);
    void AddTerritory(CTerritory *pTerritory);
    void DelTerritory(FamilyId_t FamilyID);
    void ResetMapMonster();

    void PostMsg(int32_t runnerId, GameMsgCode code, void* ptr1, void* ptr2, int32_t ip1, int32_t ip2, int32_t ip3);

    static MapManager* GetInstance();
    Map* GetMap(int32_t mapid);

private:
        int32_t m_DungenId;
        // TODO: 确认类型 m_Runner
        // TODO: 确认类型 m_Territorys
        // TODO: 确认类型 m_activitys
        // TODO: 确认类型 m_dungeons
        // TODO: 确认类型 m_mapLock
        // TODO: 确认类型 m_maps
};

#endif // _MAPMANAGER_H_