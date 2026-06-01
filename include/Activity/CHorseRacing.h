#ifndef _CHORSERACING_H_
#define _CHORSERACING_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CHorseRacing : public CActivity
{
public:
    CHorseRacing(const CfgActivity* cfgActivity);
    ~CHorseRacing();

    void reset();
    void OnUpdate(CActivityMap *pMap);
    void removePlayer(Player *player, bool islogout);
    void addPlayer(Player *player);
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    bool bArrived(CharId_t cid);
    void win(Player *player);
    void addReward(Player *player);
    void sendActivityResult(Player *player);
    void onTimeEnd();
    bool CanUsePet(MapId_t mid);
    void onMonsterDie(MonsterActivity *pMonster, Player *pKiller);
    void packetActivityScore(int8_t connid);
    void broadcastReady();
    void broadcastStart();
    void broadcastWin(Player *player);

private:
        // TODO: 确认类型 m_BossDie
        int32_t m_nIndex;
        int32_t m_nState;
        // TODO: 确认类型 m_racing
        // TODO: 确认容器类型 m_rankList
        // TODO: 确认类型 m_winner
};

#endif // _CHORSERACING_H_

