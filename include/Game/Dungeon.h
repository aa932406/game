#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>

#include "Common/CommonTypes.h"
#include "Map/Map.h"
#include "Map/IMapEvent.h"
#include "Config/CfgDungeon.h"
#include "Other/DamageSum.h"

class Monster;
class MonsterDungeon;
class NpcDungeon;
class PlantDungeon;
class Unit;
class CfgMapEvent;
class CfgMapEventList;
class CLibraryItem;

// 副本状态枚举
enum class DungeonState : int32_t
{
    DS_FREE    = 0,   // 空闲
    DS_WAIT    = 1,   // 等待
    DS_RUNNING = 2,   // 运行中
    DS_SUMMON  = 3,   // 召唤中
    DS_BOSS    = 4,   // Boss阶段
    DS_WIN     = 5,   // 胜利
    DS_FAIL    = 6,   // 失败
    DS_RESET   = 7    // 重置
};

// 副本塔信息
struct DungeonTower
{
    int32_t id;
    int32_t costType;
    int32_t costValue;
    int32_t free;
    int32_t limit;
};

// 召唤Boss信息
struct SummBossInfo
{
    CharId_t cid;
    int8_t   state;
};

// 副本奖励信息
struct DungeonReward
{
    CharId_t  cid;
    int8_t    bGet;
    int32_t   money;
    int32_t   cash;
    int32_t   exp;
    MemChrBag Item;
};

// 宝库随机配置项
struct BaoKuRandomItem
{
    int32_t nItemId;
    int32_t nItemClass;
    int32_t nGongGaoId;
};

struct BaoKuRandom
{
    int32_t Index;
    int32_t XValue;
    int32_t YValue;
    BaoKuRandomItem Items[5];
};

// 宝库副本配置
struct BaoKuFuBen
{
    int32_t AddTimes;
    int32_t InitCount;
    int32_t BuyCount;
    int32_t Gold;
};

// 副本完成信息 (32字节)
struct DungeonFinshInfo
{
    int32_t get_exp;
    int32_t reserved[7];
};

class Dungeon : public Map, public IMapEvent
{
public:
    Dungeon();
    virtual ~Dungeon();

    // Map虚函数重写
    virtual void init(CMapRunner *pRunner, const CfgMap *cfgmap) override;
    virtual void update() override;
    virtual bool canEnter(Player *player) override;
    virtual bool OnSitRevive(Player *player) override;
    virtual bool SpecialSitRevive() override;
    virtual bool CanAutoRevive() override;
    virtual bool CanUsePet() override;
    virtual bool CanStayInMap() override;
    virtual int32_t GetAvgBattle() override;
    virtual int32_t getNow() override;
    virtual int64_t getTick() override;
    virtual void onMonsterDie(Monster *monster, Player *player, bool IsGroupMonster) override;

    // IMapEvent虚函数重写
    virtual void init(const CfgMapEventList *events, int64_t nTime) override;
    virtual void openEvent(int32_t eventId) override;
    virtual void closeEvent(int32_t eventId) override;
    virtual void openEvents(int32_t *eventIds, int32_t count) override;
    virtual void closeEvents(int32_t *eventIds, int32_t count) override;

    // Dungeon特有方法
    void init(CMapRunner *pRunner, const CfgDungeon *cfgDungeon, const CfgMap *cfgmap, int32_t nId);
    void addPlayer(Player *player, int32_t x, int32_t y);
    void AddRandomList(Player *player, int32_t nIndex);
    void OnRandomPos(Player *player);
    int32_t RandPosIndex(Player *player);
    void OnBuyRandomPosTimes(Player *player);
    void reset();
    void checkEvents();
    void checkEvent(CfgMapEvent *mapEvent);
    void updateExpDungeonRank();
    void win(int32_t nParam);
    void fail(int32_t nParam);
    void triggerEvent(CfgMapEvent *mapEvent, Unit *pUnit);
    void addMonsterHPEventInfo(MonsterDungeon *pMonster);
    void checkPlayerAttrEvent(MonsterDungeon *pMonster);
    void signDungeonCompleted();
    void setCanSubmit();
    void generateMonster(const std::string *str, bool rewardWave);
    void randomMonster(const std::string *str, bool rewardWave);
    void checkAddMonster(int64_t curTick);
    void checkDropItem(MonsterDungeon *pMonster);
    bool flashWaitMonster(MonsterWait *waitAddMonster, int64_t curTick);
    void SetHard(int8_t nHard);
    void OpenEvent(int32_t nEvent);
    void setState(DungeonState state, int32_t param);
    void start(bool bFree);
    void restart(bool bFree);
    void initPlayerInfo(Player *player);
    void onPlayerLeave(Player *player);
    bool onEndDungeon(Player *player);
    void calWinStar();
    void calWinReward();
    void onMonsterHPEvent(MonsterDungeon *monster, int32_t id);
    void onPlayerHPEvent(Player *player);
    void onPlayerDie(Player *player);
    void onMonsterDie(MonsterDungeon *monster);
    int32_t onBeginGather(Player *player);
    void onPlantGather(PlantDungeon *plant);
    void OnNpcEnd(NpcDungeon *npc, int8_t choice);
    void onMonsterArriveRoadEnd(MonsterDungeon *monster);
    void OnPickItem(Player *player, const MemChrBag *item);
    std::string getDungeonName();
    PlayerList getPlayers();
    Player *GetLeader();
    const Player *GetLeader() const;
    void clearMonster();
    void broadcastFinishDungon(int32_t param);
    void sendDungeonResult(Player *player, int32_t param);
    void broadcastDungeonEvent(int32_t evtId);
    void broadcastPlayMovie(int32_t movieID);
    void broadcastDungeonInfo();
    void broadcastDamageList();
    void broadcastSummonBoss(const std::string *name, int8_t nState);
    void checkBroadcastDamageList();
    bool BuyTower(Player *player, int32_t nId);
    bool BuildTower(Player *player, int32_t nId, int16_t nPosX, int16_t nPosY);
    bool SummonBoss(Player *player, int8_t nState);
    bool Summon(Player *player, int32_t nIndex);
    void addWaitTower(const DungeonTower *tower);
    bool GetReward(Player *player, int8_t nRatio, bool IsSysTem);
    void InitTeamMember(const CharIdList *lst);
    void AddPlayerDamage(CharId_t cid, int32_t attackValue);
    void AddMonsterExp(int32_t nExp);
    void AddDungeonTime(int32_t nTime);
    int32_t GetLeftTime();
    void teamDungeonMemberEnter();
    void checkRevive(int64_t curTick);
    bool OnSafeRevive(Player *player);
    void DungeonGongGao(int8_t connid, CharId_t CharId, std::string *p_Name);
    void removePlayer(Player *player, bool islogout);
    bool isAllMonsterDie();
    bool isAllPlayerSummon();
    void checkSummonBoss();
    void sendStartSummon();
    void OpenBoxMonster(int32_t BossId, bool rewardWave);
    void sendRandEventTime();
    Position GetBackPos();
    int32_t GuessTheSize(Player *player, int32_t nId);
    bool DungeonGuWu(Player *player, int8_t Type);

    // 工具方法
    int32_t getDungeonId() const { return m_cfgDungeon.id; }
    int32_t getDungeonType() const { return m_cfgDungeon.type; }
    int32_t getDungeonGroupId() const { return m_cfgDungeon.group_id; }
    int32_t GetDurationTime() const { return m_cfgDungeon.duration + m_nAddonTime; }
    DungeonState getState() const { return m_state; }
    CfgDungeon* GetCfgDungeon() { return &m_cfgDungeon; }

private:
    CfgDungeon m_cfgDungeon;              // 副本配置
    int32_t m_nId;                        // 副本ID
    int32_t m_nHard;                      // 难度 (1-5)
    int32_t m_nLevel;                     // 等级
    int32_t m_nQuality;                   // 品质
    int32_t m_nFamilyId;                  // 家族ID
    int32_t m_nMonsterBuff;               // 怪物Buff
    int32_t m_nMonsterScore;              // 怪物积分
    int32_t m_nGatherTims;                // 采集次数
    int32_t m_TeamLeaderId;               // 队长ID
    int32_t m_nWinStar;                   // 胜利星级
    int32_t m_stateParam;                 // 状态参数
    DungeonState m_state;                 // 当前状态
    int64_t m_stateTick;                  // 状态开始tick
    int64_t m_nStartTime;                 // 开始时间
    int64_t m_nSummStart;                 // 召唤开始时间
    int64_t m_nAddonTime;                 // 附加时间
    int32_t m_BuyRandomTimes;             // 购买随机次数
    int32_t m_RandomTimes;                // 随机次数
    int32_t m_Random;                     // 随机值 (1-5)
    int32_t m_MaxRanomParam;              // 最大随机参数
    int32_t m_MaxnProbability;            // 最大概率
    int32_t m_GoldBuffLevel;              // 金币Buff等级
    int32_t m_MoneyBuffLevel;             // 铜钱Buff等级
    int64_t m_Times;                      // 次数
    bool m_bFree;                         // 是否免费
    bool m_bGetReward;                    // 是否已领奖
    bool m_needBroadcastDamage;           // 需要广播伤害
    double m_dRatio;                      // 倍率

    std::map<int, MonsterWave> m_monsterWave;           // 怪物波次
    std::list<MonsterWait> m_waitMonster;               // 等待刷出的怪物
    std::list<DungeonTower> m_waitTower;                // 等待建造的塔
    std::list<Monster *> m_buildTower;                  // 已建造的塔(怪物形式)
    std::list<DungeonReward> m_playerReward;            // 玩家奖励列表
    CharIdList m_memberList;                            // 队伍成员列表
    std::list<DamageSum> m_damages;                     // 伤害统计
    std::list<int> m_waitEvent;                         // 等待事件
    std::list<SummBossInfo> m_lSummonBoss;              // 召唤Boss列表
    std::map<int, int> m_mSummonRecord;                 // 召唤记录
    std::vector<MemChrBag> m_vPickItems;                // 拾取物品
    std::vector<CLibraryItem> m_LibraryRewardVector;    // 图书馆奖励
    std::list<int> m_RandomList;                        // 随机列表
    std::map<int, BaoKuRandom> m_RandomMap;             // 随机映射
    DungeonFinshInfo m_finshInfo;                       // 完成信息(32字节)
};


#endif // _DUNGEON_H_
