#pragma once
#include <cstdint>
#include <list>

#include "Answer/Mutex.h"

class Map;
class GameMsg;

namespace Answer {
    class Thread {
    public:
        Thread();
        virtual ~Thread();
        virtual void run();
    protected:
        int (**_vptr_Thread)(...);
    };
    
    template<typename T>
    class SafeQueue {
    public:
        SafeQueue();
        std::list<T> m_queue;
        int32_t m_size;
        Answer::Mutex m_lock;
    };
}

#ifndef CMAPRUNNER_CLASS_DEFINED
#define CMAPRUNNER_CLASS_DEFINED
class CMapRunner : public Answer::Thread
{
public:
    CMapRunner();
    virtual ~CMapRunner();

    virtual void run();
    int64_t getLocalNow();
    void Stop();
    void AddMap(Map *map);
    void DelMap(Map *map);
    void PostMsg(GameMsg *msg);
    void update();
    void checkMessage();
    void dealMessage(GameMsg *msg);
    void onPlayerEnterMap(Player *player, int32_t mapId);
    void onPlayerLeaveMap(Player *player);
    void onAddDungeon(Dungeon *dungeon);
    void onDelDungeon(int32_t dungeonId);
    void onPlayerLogout(Player *player);
    void onAddTerritory(CTerritory *territory);
    void onDelTerritory(int64_t id);
    void onActivityStart(int32_t activityId, int32_t mapId);
    void onActivityStop(int32_t activityId);
    void onStartGoblinRuqin(int32_t activityId, int32_t mapId, int32_t count);
    void onStartRefreshMonster(int32_t activityId, int32_t mapId);
    void onStartActivityWorldBoss(int32_t activityId, int32_t mapId);
    void onStarCityWar(int32_t activityId, int32_t mapId);

protected:
    std::list<Map *> m_maps;
    Answer::SafeQueue<GameMsg *> m_msgQueue;
};
#endif // CMAPRUNNER_CLASS_DEFINED
