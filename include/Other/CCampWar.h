#ifndef _CCAMPWAR_H_
#define _CCAMPWAR_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CCampWar : public CActivity
{
public:
    CCampWar(const CfgActivity* cfgActivity);
    ~CCampWar();

    void OnUpdate(CActivityMap *pMap);
    void reset();
    void onPlayerKilled(Player *pDier, Player *pAttacker);
    void onMonsterDie(MonsterActivity *pMonster, Player *pKiller);
    void addKillReward(CharId_t nCharId, int32_t nKillCount);
    void onPlayerRevive(Player *player, bool bSafe);
    void onPlantGather(PlantActivity *pPlant, Player *player);
    void RefreshBuff(int32_t BuffId, int8_t CampId);
    void broadcastActivityResult();
    int32_t onBeginGather(PlantActivity *pPlant, Player *player);
    void addPlayer(Player *player);
    void removePlayer(Player *player, bool islogout);
    void addPlayerScore(Player *player, int32_t nPoint, int32_t nKillCount);
    void refreshRankIndex(CharId_t nCharId);
    void addRewards();
    void onTimeEnd();
    void onWarEnd();
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void packetActivityScore(int8_t connid);
    void sendPlayerScore(Player *player);
    void SendPlayerRankInfo(Player *player);
    void broadcastReady();
    void broadcastStart();
    int32_t selectCamp(Player *player);
    int32_t getContTitle(int32_t nContLevel);
    void notifyAddScore(Player *player, int32_t nPoint, int32_t nKillCount);

private:
        int32_t m_BuffIdMap;
        // TODO: 确认类型 m_Camp_1_battle
        // TODO: 确认类型 m_Camp_2_battle
        // TODO: 确认类型 m_mPlayerScore
        int64_t m_nLastTime;
        int32_t m_nState;
        // TODO: 确认类型 m_players
        // TODO: 确认类型 m_sScoreRank
};

#endif // _CCAMPWAR_H_

