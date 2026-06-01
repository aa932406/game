#ifndef _ULTIMATECHALLENGE_H_
#define _ULTIMATECHALLENGE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class UltimateChallenge : public CActivity
{
public:
    UltimateChallenge(const CfgActivity* cfgActivity);
    ~UltimateChallenge();

    void OnUpdate(CActivityMap *pMap);
    void onMonsterDie(MonsterActivity *pMonster, Player *pKiller);
    void BroadcastActivityState();
    int8_t GetState();
    void reset();
    int8_t getIconState();
    int32_t GetIconLeftTime();
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void removePlayer(Player *player, bool islogout);
    void addPlayer(Player *player);
    void onPlayerRevive(Player *player, bool bSafe);
    void broadcastReady();
    void broadcastStart();
    void onPlayerKilled(Player *pDier, Player *pKiller);

private:
        int32_t m_ActState;
        // TODO: 确认类型 m_MonsterScore
        int64_t m_lastUpdateTime;
        // TODO: 确认类型 m_nRand
        int32_t m_nState;
};

#endif // _ULTIMATECHALLENGE_H_

