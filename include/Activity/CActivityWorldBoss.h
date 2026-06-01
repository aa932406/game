#ifndef __CACTIVITY_WORLD_BOSS_H__
#define __CACTIVITY_WORLD_BOSS_H__

#include "Activity/CActivity.h"
#include <map>
#include <set>
#include <string>
#include <cstdint>

class Player;
class CActivityMap;
class MonsterActivity;

#ifndef WORLD_BOSS_PLAYER_SCORE_DEFINED
#define WORLD_BOSS_PLAYER_SCORE_DEFINED
struct WorldBossPlayerScore
{
    int64_t nCharId;
    std::string strName;
    int32_t nDamage;
    int32_t nIndex;
    int32_t bInActivity;
    int64_t nStartTick;
    int64_t DamageTick;
    int8_t connId;
    
    WorldBossPlayerScore() : nCharId(0), nDamage(0), nIndex(0), bInActivity(0), 
                    nStartTick(0), DamageTick(0), connId(0) {}
};
#endif

struct WorldBossPlayerScoreRankGreater
{
    bool operator()(const WorldBossPlayerScore* lhs, const WorldBossPlayerScore* rhs) const;
};

class CActivityWorldBoss : public CActivity
{
public:
    explicit CActivityWorldBoss(const CfgActivity* const cfgActivity);
    virtual ~CActivityWorldBoss();

    // 重写基类虚函数
    virtual int32_t GetRevive(Player* player);
    virtual bool OnSitRevive(Player* player);
    virtual void OnUpdate(CActivityMap* pMap);
    virtual void AddPlayer(Player* player);
    virtual bool CanEnter(Player* player, CActivityMap* pTargetMap);
    virtual void RemovePlayer(Player* player, bool islogout);
    virtual void Reset();
    virtual void OnTimeEnd();
    virtual bool NeedBroadcastActivityScore(CActivityMap* pMap);
    virtual void OnMonsterAdd(MonsterActivity* pMonster);
    virtual void OnMonsterDamaged(MonsterActivity* pMonster, uint32_t damage, Player* pAttacker);
    virtual void OnMonsterDie(MonsterActivity* pMonster, Player* pKiller);
    virtual void OnPlayerKilled(Player* pDier, Player* pKiller);

    // 世界BOSS特有方法
    void AddPlayerScore(Player* player, int64_t damage);
    void AddRewards();
    void BroadcastActivityResult();
    void BroadcastBossKilled(const std::string& name, int64_t cid);
    void BroadcastStart();
    void BroadcastEnd();
    void RefreshRank();
    void SendPlayerScore(Player* player);
    int32_t GetAutoReviveTime(Player* player);
    WorldBossPlayerScore* GetRankFirst();

protected:
    // 玩家分数记录
    std::map<int64_t, WorldBossPlayerScore> m_mPlayerScore;
    
    // 分数排名（按伤害降序）
    std::multiset<WorldBossPlayerScore*, WorldBossPlayerScoreRankGreater> m_sScoreRank;
    
    // 玩家复活次数
    std::map<int64_t, int32_t> m_ReviveTimes;
    
    // BOSS是否死亡
    int8_t m_IsBossDie;
};

#endif // __CACTIVITY_WORLD_BOSS_H__