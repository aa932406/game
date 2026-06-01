#ifndef _CCITYWAR_H_
#define _CCITYWAR_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Common/CommonTypes.h"
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CCityWar : public CActivity
{
public:
    CCityWar(const CfgActivity* cfgActivity);
    ~CCityWar();

    void Init();
    void getFamilyMembers(FamilyId_t nFamilyId, CharIdList * cids);
    void OnUpdate(CActivityMap *pMap);
    void reset();
    bool isSpecialDay(int32_t nStartDays);
    bool isInSpecialDay(int32_t nStartDays);
    bool checkData();
    bool HaveOwner();
    void checkChangeFamily(bool IsUpdateScore);
    void NotOccupyGongGao();
    void SetOwner();
    FamilyId_t GetGuilDerFirstFamily();
    bool checkWeek();
    void onMonsterDie(MonsterActivity *pMonster, Player *pKiller);
    void onPlayerKilled(Player *pDier, Player *pAttacker);
    void onMonsterAdd(MonsterActivity *pMonster);
    int8_t GetFamilyCamp(FamilyId_t FamilyId);
    void addPlayer(Player *player);
    void removePlayer(Player *player, bool islogout);
    bool canGetOnlineReward(struct PlayerScore * score);
    void SendPlayerActivityInfo(Player *player);
    void SendPlayerActivityScore(Player *player);
    Position GetRandBornPos(Player *player);
    int32_t GetRevive(Player *player);
    void addPlayerScore(Player *player, int32_t nScore);
    void setUpdateScoreRank(FamilyId_t nFamilyId, bool bScoreRank);
    void addFamilyScore(FamilyId_t nFamilyId, int32_t nScore, int32_t nNowTime);
    void win();
    void addRewards();
    void saveResult();
    void onActivityStart();
    void onTimeEnd();
    int32_t getNextStartTime();
    void SendAppyCityWarInfo(Player *player);
    bool ApplyCityWar(Player *player, FamilyId_t FamilyId, int8_t Type);
    void AutoApplyCityWar();
    void GongGao(FamilyId_t FamilyId, int32_t GongGaoId);
    void SaveApplyInfo();
    int32_t canEnter(Player *player, CActivityMap *pTargetMap);
    void packetActivityScore(int8_t connid);
    void broadcastReady();
    void broadcastStart();
    void OnCityWarResult(int64_t nFamilyId, int32_t nWinTime, int32_t nTime,
                         int64_t nLeaderCid, int32_t First, int32_t Second,
                         int32_t Third, int32_t nIndex,
                         const std::string& p_FirstFamilyName,
                         const std::string& p_SecondFamilyName,
                         const std::string& p_ThirdFamilyName);

private:
        // TODO: 确认类型 m_Apply
        // TODO: 确认类型 m_First
        // TODO: 确认类型 m_FirstFamilyName
        // TODO: 确认类型 m_Leader
        // TODO: 确认类型 m_Second
        // TODO: 确认类型 m_SecondFamilyName
        // TODO: 确认类型 m_Third
        // TODO: 确认类型 m_ThirdFamilyName
        // TODO: 确认类型 m_bUpdateFamilyScroe
        // TODO: 确认类型 m_lFamilyScore
        // TODO: 确认类型 m_mFamilyScore
        // TODO: 确认类型 m_mPlayerScore
        int32_t m_nFamilyId;
        int32_t m_nIndex;
        int32_t m_nLastFamilyId;
        int64_t m_nLastUpdateRankTick;
        int64_t m_nStartTime;
        int32_t m_nState;
        int64_t m_nWinTime;
        // TODO: 确认类型 m_pMonster
        // TODO: 确认类型 m_players
};

#endif // _CCITYWAR_H_

