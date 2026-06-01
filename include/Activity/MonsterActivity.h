#ifndef __MONSTER_ACTIVITY_H__
#define __MONSTER_ACTIVITY_H__

#include "Game/Monster.h"
#include "Config/CfgActivityMonster.h"
#include <list>
#include <string>
#include <cstdint>

class CActivityMap;
class Player;
class CfgActivityMonster;
class CfgMonster;
class CfgMapMonster;

// MonsterActivity-specific HPEvent (different from Common/CommonTypes.h)
#ifndef MONSTERACTIVITY_HP_EVENT_DEFINED
#define MONSTERACTIVITY_HP_EVENT_DEFINED
struct MonsterActivityHPEvent
{
    int32_t id;
    int32_t minhp;
    int32_t maxhp;
    
    MonsterActivityHPEvent() : id(0), minhp(0), maxhp(0) {}
};
#endif

class MonsterActivity : public Monster
{
public:
    MonsterActivity();
    virtual ~MonsterActivity();

    // 初始化与清理
    void init(CActivityMap* pActivityMap, 
              const CfgActivityMonster* cfgActivityMonster,
              const CfgMonster* cfgmonster,
              const CfgMapMonster* cfgmapmonster);
    void reset();
    void destroy();
    void remove();
    
    // 生命周期
    bool refresh();
    void die();
    void onArriveTarget();
    void postDamage(int32_t damage, class UnitHandle launcher, int32_t Mid);
    
    // 移动
    void runOnRoad();
    
    // 事件
    void addEventHp(const MonsterActivityHPEvent* hpEvent);
    void checkHpEvent(int64_t curTick);
    void checkFamilyMoney(int64_t curTick);
    
    // 家族相关
    void AddFamilyMoney(int32_t money);
    int32_t GetFamilyMoney() const;
    void setFamilyId(int64_t nFamilyId);
    int64_t getFamilyId() const;
    
    // 击杀者
    void setKiller(int64_t nKiller);
    
    // 广播
    void broadcastBasicData();
    
    // Getter
    int32_t GetActivityMonsterId() const;
    bool IsBoss() const;
    int32_t GetBossSign() const;
    void SetSyncTime(int32_t ms);

protected:
    CActivityMap* m_pActivityMap;
    std::list<MonsterActivityHPEvent> m_hpEvents;
    std::list<Position> m_road;
    CfgActivityMonster m_cfgActivityMoster;
    int64_t m_nOwner;
    int32_t m_nFamilyMoney;
    int64_t m_nFamilyMoneyTick;
    int64_t m_nFamilyId;
    LauncherInfo m_killer;
    int32_t m_nSyncInterval;
};

#endif // __MONSTER_ACTIVITY_H__