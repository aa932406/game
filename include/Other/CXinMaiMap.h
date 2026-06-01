#ifndef _CXINMAIMAP_H_
#define _CXINMAIMAP_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CXinMaiMap : public CActivity
{
public:
    CXinMaiMap(const CfgActivity* cfgActivity);
    ~CXinMaiMap();

    void OnUpdate(CActivityMap *pMap);
    void onPlayerKilled(Player *pDier, Player *pKiller);
    bool OnSitRevive(Player *player);
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void reset();
    void addPlayer(Player *player);
    void removePlayer(Player *player, bool islogout);
    void sendPlayerScore(Player *player);
    void packetActivityScore(int8_t connid);
    void onTimeEnd();
    void broadcastReady();
    void broadcastStart();
    void onMonsterAdd(MonsterActivity *pMonster);
    int32_t onBeginGather(PlantActivity *plant, Player *player);
    void onPlantGather(PlantActivity *pPlant, Player *player);
    bool alwaysShowIcon();
    void onMonsterDie(MonsterActivity *pMonster, Player *pKiller);

private:
        int64_t m_LeftTime;
        // TODO: 确认类型 m_MonsterActivityMap
        int64_t m_ReviveTimes;
        int32_t m_nState;
        // TODO: 确认类型 m_pLastBoss
};

#endif // _CXINMAIMAP_H_

