#pragma once
#include <cstdint>
#include <map>

class CActivity;
class CActivityMap;
class CfgActivity;
class MonsterActivity;
class Player;

class CXinMaiMap : public CActivity
{
public:
    CXinMaiMap(const CfgActivity &cfgActivity);
    virtual ~CXinMaiMap();

    virtual void OnUpdate(CActivityMap *pMap);
    virtual void onPlayerKilled(Player *pDier, Player *pKiller);
    virtual bool OnSitRevive(Player *player);
    virtual bool canEnter(Player *player);
    virtual void reset();
    virtual void addPlayer(Player *player);
    virtual void removePlayer(Player *player);
    virtual void onTimeEnd();
    virtual void onMonsterAdd(MonsterActivity *monster);
    virtual void onBeginGather(PlantActivity *plant, Player *player);
    virtual void onPlantGather(PlantActivity *plant, Player *player);
    virtual bool alwaysShowIcon();
    virtual void onMonsterDie(MonsterActivity *pMonster, Player *killer);

    void sendPlayerScore(Player *player, int32_t addScore);
    void packetActivityScore(Answer::NetPacket *packet);
    void broadcastReady();
    void broadcastStart();

protected:
    std::map<int64_t, MonsterActivity *> m_MonsterActivityMap;
    std::map<int64_t, int32_t> m_ReviveTimes;
};
