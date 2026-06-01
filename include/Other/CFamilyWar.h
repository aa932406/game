#ifndef _CFAMILYWAR_H_
#define _CFAMILYWAR_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CFamilyWar : public CActivity
{
public:
    CFamilyWar(const CfgActivity* cfgActivity);
    ~CFamilyWar();

    void OnUpdate(CActivityMap *pMap);
    void reset();
    void onMonsterDie(MonsterActivity *pMonster, Player *pKiller);
    void onPlayerKilled(Player *pDier, Player *pAttacker);
    void onMonsterAdd(MonsterActivity *pMonster);
    void AdjustMonsterAttr(CfgMonster * cfgMonster);
    void addPlayer(Player *player);
    void removePlayer(Player *player, bool islogout);
    void addPlayerScore(Player *player, int32_t nScore, int32_t nKillCount);
    void win(FamilyId_t nFamilyId, std::string * strFamilyName);
    void sendPlayerScoreRankReward();
    void sendFamilyScoreRankReward();
    void sendActivePillarReward(FamilyId_t nFamilyId);
    void sendWinnerReward(FamilyId_t nFamilyId);
    void onTimeEnd();
    void packetActivityScore(int8_t connid);
    void appendPlayerScoreRank(Answer::NetPacket *packet);
    void appendFamilyScoreRank(Answer::NetPacket *packet);
    void sendPlayerScore(Player *player);
    void broadcastReady();
    void broadcastStart();
    void broadcastPillerKilled(FamilyId_t nFamilyId);
    void broadcastStoneKilled();
    void addBuff(Player *player);
    void removeBuff(Player *player);
    bool OnActivePillar(Player *player, int32_t nIndex);
    void OnTaskSubmited(Player *player, int32_t nScore);
    int32_t GetActivePillar(FamilyId_t nFamilyId);
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void getPlayerScore(CharId_t nCharId);
    void getFamilyScore(FamilyId_t nFamilyId);

private:
        // TODO: 确认类型 m_lstFamilyScore
        // TODO: 确认类型 m_lstPlayerScore
        int32_t m_nActivePillarState;
        int32_t m_nActiveState;
        int64_t m_nBuffStartTick;
        // TODO: 确认类型 m_nWinFamily
        // TODO: 确认类型 m_pBoss
        // TODO: 确认类型 m_pBuffPlayer
        // TODO: 确认类型 m_pTitle
        // TODO: 确认类型 m_strWingFamily
};

#endif // _CFAMILYWAR_H_

