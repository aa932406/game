#ifndef _CROSSTOWER_H_
#define _CROSSTOWER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CrossTower : public CActivity
{
public:
    CrossTower(const CfgActivity* cfgActivity);
    ~CrossTower();

    void Init();
    void OnUpdate(CActivityMap *pMap);
    void packetActivityMapInfo(CActivityMap *pMap);
    int32_t onBeginGather(PlantActivity *plant, Player *player);
    void onPlantGather(PlantActivity *pPlant, Player *player);
    Position GetRandBornPos(Player *player);
    void reset();
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void addPlayer(Player *player);
    void removePlayer(Player *player, bool islogout);
    void broadcastReady();
    void broadcastStart();
    void onTimeEnd();
    void GiveDailyReward(Player *player);
    void addRewards();
    void broadcastActivityResult();
    void sendPlayerScore(Player *player);
    void onPlayerKilled(Player *pDier, Player *pKiller);
    void packetActivityScore(int8_t connid);
    void onMonsterAdd(MonsterActivity *pMonster);
    void onMonsterDie(MonsterActivity *pMonster, Player *pKiller);
    void ActUpdate(int64_t CurTick);
    int32_t GetEnterMapId(Player *player);
    int32_t GetRevive(Player *player);
    void onPlayerRevive(Player *player, bool bSafe);
    void CheckWin(bool IsTimeEnd);
    void SetWinnerInfo(CharId_t Winner, const std::string& p_sName);

private:
        // TODO: 确认类型 m_Battle
        int32_t m_ConnId;
        int64_t m_LastAddScoreTime;
        int64_t m_LastUpdateTime;
        // TODO: 确认类型 m_Monster
        // TODO: 确认类型 m_NeedAddScore
        // TODO: 确认类型 m_NeedChangeMap
        // TODO: 确认类型 m_NeedSysMap
        int32_t m_NewWinerId;
        // TODO: 确认类型 m_NewWinerName
        int32_t m_State;
        int64_t m_StateTime;
        int32_t m_WinerId;
        // TODO: 确认类型 m_WinerName
        // TODO: 确认类型 m_mPlayerScore
        int32_t m_nState;
        // TODO: 确认类型 m_players
};

#endif // _CROSSTOWER_H_

