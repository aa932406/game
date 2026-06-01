#ifndef _CFAMILYLIGHT_H_
#define _CFAMILYLIGHT_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CFamilyLight : public CActivity
{
public:
    CFamilyLight(const CfgActivity* cfgActivity);
    ~CFamilyLight();

    void OnUpdate(CActivityMap *pMap);
    void onPlayerKilled(Player *pDier, Player *pKiller);
    void AddScore(Player *pPlayer, int32_t Money, int32_t Score);
    bool OnSitRevive(Player *player);
    void reset();
    void addPlayer(Player *player);
    void removePlayer(Player *player, bool islogout);
    void sendPlayerScore(Player *player);
    void broadcastActivityResult();
    bool DrinkWine(Player *player, int32_t Count);
    void onTimeEnd();
    void broadcastReady();
    void broadcastStart();

private:
        int64_t m_LastUpdateTick;
        // TODO: 确认类型 m_mPlayerScore
        int32_t m_nState;
};

#endif // _CFAMILYLIGHT_H_

