#pragma once
#include <cstdint>
#include <map>

#include "Answer/Mutex.h"
#include "Other/GameMsg.h"
#include "Other/CMapRunner.h"

class Map;
class Dungeon;
class CActivityMap;
class CTerritory;

namespace Answer {
    class Random {
    public:
        static Random* GetInstance();
        static Random* instance();
        static int32_t rand(int32_t min, int32_t max);
        static int32_t generate(Random* r, int32_t min, int32_t max);
        int32_t generate(int32_t min, int32_t max);
        int32_t Rand(int32_t n);
    };
}

class MapManager
{
public:
    MapManager();
    ~MapManager();

    void StartAll();
    void StopAll();
    void initRunner();
    void initMap(const CfgMap *cfgmap);
    Map *GetMap(int32_t mapId);
    void AddDungeon(Dungeon *dungeon);
    Dungeon *NewDungeon(int32_t dungeonId);
    void DelDungeon(int32_t dungeonId);
    void PostMsg(int32_t runnerId, GameMsgCode code, void* ptr1, void* ptr2, int32_t ip1, int32_t ip2, int32_t ip3);

    static MapManager* GetInstance();
    int32_t newDungenId();
    Dungeon *GetDungeon(int32_t dungeonId);
    CTerritory *GetTerritory(int64_t id);
    CTerritory *NewTerritory(int64_t id);
    void AddTerritory(CTerritory *territory);
    void DelTerritory(int64_t id);
    void ResetMapMonster(int32_t mapId);

protected:
    std::map<int, Map *> m_maps;
    std::map<int, Dungeon *> m_dungeons;
    std::map<int, CActivityMap *> m_activitys;
    std::map<int64_t, CTerritory *> m_Territorys;
    CMapRunner m_Runner[5];
    Answer::Mutex m_mapLock;
    int32_t m_DungenId;
};
