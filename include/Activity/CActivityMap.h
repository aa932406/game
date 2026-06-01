#ifndef __CACTIVITY_MAP_H__
#define __CACTIVITY_MAP_H__

#include "Common/CommonTypes.h"
#include "Map/Map.h"
#include "Map/IMapEvent.h"
#include <list>
#include <map>
#include <vector>
#include <string>
#include <cstdint>

class CActivity;
class Player;
class MonsterActivity;
class PlantActivity;
class NpcActivity;
class CfgMapEvent;
struct LauncherInfo;
struct MonsterWait;
struct HPEvent;

class CActivityMap : public Map, public IMapEvent
{
public:
    CActivityMap();
    virtual ~CActivityMap();

    // 初始化与清理
    void Init(CMapRunner* pRunner, const CfgMap* const cfgmap);
    void Clear();
    
    // 活动关联
    void OnActivityStart(CActivity* pActivity);
    CActivity* GetActivity() const { return m_pActivity; }
    
    // 玩家相关
    virtual bool CanEnter(Player* player) override;
    virtual void AddPlayer(Player* player, int32_t x, int32_t y);
    virtual void RemovePlayer(Player* player, bool islogout);
    virtual bool OnSitRevive(Player* player) override;
    virtual bool CanSitRevive(Player* player) override;
    virtual bool SpecialSitRevive(Player* player) override;
    virtual int32_t GetReive(Player* player) override;
    virtual int32_t GetPkMode() override;
    
    // 怪物相关
    void CheckAddMonster(int64_t curTick);
    bool FlashMonster(const MonsterWait& waitMonster, int64_t curTick);
    void GenerateMonster(const std::string& str);
    int32_t HaveAliveMonster();
    int32_t HaveAlivePet();
    void AddMonsterHPEventInfo(MonsterActivity* pMonster, const CfgMapEvent* mapEvent);
    
    // 事件相关
    void CheckEvents(int64_t curTick);
    void CheckEvent(CfgMapEvent* mapEvent, int64_t curTick);
    void TriggerEvent(CfgMapEvent* mapEvent, void* target);
    
    // NPC相关
    void AddActivityNpc(const std::string& Effect);
    
    // 植物相关
    void OnBeginGather(PlantActivity* plant, Player* player);
    void OnPlantGather(PlantActivity* plant, Player* player);
    
    // 怪物回调
    void OnMonsterDamaged(MonsterActivity* pMonster, Player* player, int64_t damage, const LauncherInfo* launcher);
    void OnMonsterDie(MonsterActivity* pMonster, Player* killer);
    void OnMonsterHPEvent(MonsterActivity* pMonster, int32_t eventId);
    void OnMonsterArriveRoadEnd(MonsterActivity* pMonster) {}
    
    // 玩家回调
    void OnPlayerKilled(Player* killer, Player* victim);
    void OnPlayerRevive(Player* player, bool bSafe);
    
    // 其他
    void SendUltimateChallengeInfo(Player* player);
    int32_t GetTop10Battle();
    int64_t GetTick();
    bool IsActivityMap();
    
    virtual void Update(int64_t curTick) override;

protected:
    CActivity* m_pActivity;
    int64_t m_nStartTick;
    std::map<int32_t, MonsterWave> m_monsterWave;
    std::list<MonsterWait> m_waitMonster;
    std::list<MonsterActivity*> m_actMonsters;
    std::list<PlantActivity*> m_actPlants;
    std::map<int64_t, int32_t> m_CidBattle;
};

#endif // __CACTIVITY_MAP_H__