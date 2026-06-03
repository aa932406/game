#ifndef __CACTIVITY_H__
#define __CACTIVITY_H__

#include <list>
#include <map>
#include <vector>
#include <string>
#include <cstdint>

class Player;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class MonsterActivity;
class PlantActivity;
namespace Answer { class NetPacket; }

enum ACTIVITY_STATE : int32_t
{
    AS_NOT_START = 0,
    AS_READY = 1,
    AS_RUNNING = 2,
    AS_END = 3,
    AS_TIME_OUT = 4
};

struct ShowIcon
{
    int32_t nId;
    int32_t nState;
    int32_t nLeftTime;
    int32_t nEffects;
    int32_t nIconRight;
};

struct LauncherInfo
{
    int64_t id;
    int32_t type;
};

class CActivity
{
public:
    explicit CActivity(const CfgActivity* const cfgActivity);
    virtual ~CActivity();

    // 核心业务方法
    virtual void CheckActivity();
    virtual void Init();
    virtual void Reset();
    virtual void StartActivity();
    virtual void StopActivity();
    virtual void OnTimeEnd();
    
    // 玩家相关
    virtual void AddPlayer(Player* player);
    virtual void RemovePlayer(Player* player, bool islogout = false);
    virtual bool CanEnter(Player* player, CActivityMap* pTargetMap);
    virtual void SendPlayerActivityInfo(Player* player);
    virtual void SendPlayerActivityScore(Player* player);
    
    // 地图相关
    virtual void OnUpdate(CActivityMap* pMap);
    virtual void OnChangeMap(CActivityMap* pMap, Player* player, int32_t nX, int32_t nY);
    virtual void CheckRevive(CActivityMap* pMap);
    
    // 状态相关
    virtual int32_t GetIconState();
    virtual void GetIconState(std::list<ShowIcon>& iconList);
    virtual void BroadcastActivityState();
    virtual void BroadcastActivityScore(Player* player, int32_t score);
    
    // 时间相关
    virtual int32_t GetLeftTime();
    virtual int32_t GetNextStartTime();
    virtual int64_t GetActivityTime();
    virtual void DelayKickAll(int32_t nTime);
    
    // 其他
    virtual int32_t GetRevive(Player* player);
    virtual void AddActivityBuff(Player* player, int32_t nBuffId, bool bClear);
    virtual void AdjustMonsterAttr(CfgMonster* cfgMonster, int32_t nLevel);
    virtual void SetNeedBroadcastActivityScore(bool b);
    void setNeedBroadcastActivityScore(bool b) { SetNeedBroadcastActivityScore(b); }
    virtual bool NeedBroadcastActivityScore();
    virtual void RemoveActivityMonster(int32_t monsterId);
    virtual void SendSocialUpdateActivityState(int8_t nState);
    
    // 数据验证
    virtual bool CheckData();
    virtual bool CheckLine();
    virtual bool CheckWeek();
    virtual bool IsRightTime();
    
    // Getter/Setter
    int32_t GetType();
    int32_t GetId();
    bool IsRuning();
    virtual bool IsCrossActivity();
    virtual bool IsRightLine();
    ACTIVITY_STATE GetState() const { return m_nState; }
    void SetState(ACTIVITY_STATE state) { m_nState = state; }
    
    // 子类可重写的虚函数
    virtual bool CheckBeginTimeValid() { return true; }
    virtual bool CheckEndTimeValid() { return true; }
    virtual bool IsActivityMap(CActivityMap* pMap);
    virtual ACTIVITY_STATE GetActivityStateByTime(tm* time);
    virtual int32_t GetIconStateInternal();
    virtual int32_t GetReviveTime(Player* player);
    virtual void OnActivityReady();
    virtual void OnActivityStart();
    virtual void OnActivityFinish();
    virtual void OnActivityEnd();
    virtual void OnActivityStarted();
    virtual void OnMapUpdate(CActivityMap* pMap);
    virtual bool NeedCheckRevive() { return true; }
    virtual void OnPlayersEmpty();
    virtual void OnPlayerAdded(Player* player);
    virtual void OnPlayerScoreSent(Player* player);
    virtual Answer::NetPacket* CreateScorePacket(Player* player, int32_t score);
    virtual void OnPlayerEnter(Player* player) {}
    virtual void OnPlayerLeave(Player* player, bool islogout) {}
    virtual void OnMonsterAdd(MonsterActivity* pMonster) {}
    virtual void OnMonsterDamaged(MonsterActivity* pMonster, uint32_t damage, Player* pAttacker) {}
    virtual void OnMonsterDie(MonsterActivity* pMonster, Player* pKiller) {}
    virtual void OnPlayerKilled(Player* pDier, Player* pKiller) {}
    virtual void OnPlayerRevive(Player* player, bool bSafe) {}
    virtual void OnBeginGather(PlantActivity* plant, Player* player) {}
    virtual void OnPlantGather(PlantActivity* plant, Player* player) {}
    virtual bool CanSitRevive() { return false; }
    virtual bool SpecialSitRevive() { return false; }
    virtual bool OnSitRevive(Player* player) { return false; }
    virtual int32_t GetPkMode() { return 0; }
    virtual void UpdateActivityState(int32_t nState) {}
    virtual void OnUpdateGlobal(int64_t curTick) {}
    virtual void SendActivityRankInfo(Player* player) {}
    virtual void GiveDailyReward(Player* player) {}
    virtual int32_t HaveRewardCount(Player* player) { return 0; }

protected:
    const CfgActivity* m_cfgActivity;
    ACTIVITY_STATE m_nState;
    std::list<CActivityMap*> m_activityMaps;
    std::list<Player*> m_players;
    int32_t m_nStartTime;
    int32_t m_nKickTime;
    int32_t m_nBraodcastActivityScoreSign;
    int64_t m_nBroadcastActivityScoreTick;
    int64_t m_nLastReviveCheckTick;
    
private:
    int32_t GetNextStartTimeInternal();
};

// ACTIVITY_STATE 隐式转换操作符
inline int32_t operator+(ACTIVITY_STATE s) { return static_cast<int32_t>(s); }
inline bool operator==(int32_t a, ACTIVITY_STATE b) { return a == static_cast<int32_t>(b); }
inline bool operator==(ACTIVITY_STATE a, int32_t b) { return static_cast<int32_t>(a) == b; }
inline bool operator!=(int32_t a, ACTIVITY_STATE b) { return a != static_cast<int32_t>(b); }
inline bool operator!=(ACTIVITY_STATE a, int32_t b) { return static_cast<int32_t>(a) != b; }

#endif // __CACTIVITY_H__