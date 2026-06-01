#ifndef _CTIANJIANGBAOXIANG_H_
#define _CTIANJIANGBAOXIANG_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CTianJiangBaoXiang : public CActivity
{
public:
    ~CTianJiangBaoXiang();

    void addPlayer(Player *player);
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void reset();
    void sendPlayerScore(Player *player);
    void AddPlant(PlantActivity *plant);
    int32_t GiveDailyReward(Player *player);
    void onPlantGather(PlantActivity *pPlant, Player *player);
    int32_t onBeginGather(PlantActivity *plant, Player *player);
    bool AddOpenCount(Player *player, int32_t AddCount);
    void broadcastReady();
    void broadcastStart();

private:
        // TODO: 确认类型 m_PlayerSoreMap
        int32_t m_nState;
        // TODO: 确认类型 m_players
};

#endif // _CTIANJIANGBAOXIANG_H_

