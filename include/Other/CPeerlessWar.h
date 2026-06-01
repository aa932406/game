#ifndef _CPEERLESSWAR_H_
#define _CPEERLESSWAR_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CPeerlessWar : public CActivity
{
public:
    CPeerlessWar(const CfgActivity* cfgActivity);
    ~CPeerlessWar();

    void Init();
    void OnUpdate(CActivityMap *pMap);
    void reset();
    void onPlayerKilled(Player *pDier, Player *pAttacker);
    void onPlayerRevive(Player *player, bool bSafe);
    void addPlayer(Player *player);
    void removePlayer(Player *player, bool islogout);
    void sendPlayerScore(Player *player);
    void packetActivityScore(int8_t connid);
    void sendActivityResult(Player *player, int8_t nRes, int32_t KillCount);
    Position GetRandBornPos(Player *player);
    int32_t getLeftReviveTimes(Player *player);
    void win(Player *pWinner);
    void draw();
    void addWinnerReward();
    void addJoinReward(CharId_t nCharId);
    void addKillReward(CharId_t nCharId, int32_t nKillCount);
    void addRewards();
    void saveWarResult();
    void clearOldTitle();
    bool needBroadcastContKill(int32_t nCountKill);
    bool needBroadcastBreakCont(int32_t nCountKill);
    void addPlayerKillCount(Player *player);
    void onTimeEnd();
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void broadcastReady();
    void broadcastStart();
    void broadcastKnockout();
    void broadcastWin(std::string * name, CharId_t cid);
    void broadcastDraw();
    bool checkWin();
    std::string GetWinnerName();

private:
        // TODO: 确认类型 m_mPlayerScore
        int64_t m_nLastTick;
        int64_t m_nStartTime;
        int32_t m_nState;
        int32_t m_nWarState;
        // TODO: 确认类型 m_nWinner
        // TODO: 确认类型 m_players
        // TODO: 确认类型 m_strWinnerName
};

#endif // _CPEERLESSWAR_H_

