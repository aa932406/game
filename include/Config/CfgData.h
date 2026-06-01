#ifndef CFG_DATA_H
#define CFG_DATA_H

#include <list>
#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <mutex>

// 前向声明（根据实际项目调整）
class CDBCFile;
class GameService;
class ItemEffectManager;
class COpenBeta;
class CCardGroupBoxManager;
class CItemHelper;

namespace Answer {
    template<typename T> class Singleton;
    class Logger;
    class DayTime;
    class DBPool;
    class MySqlDBGuard;
    class MySqlQuery;
    class NetPacket;
    class Random;
    class StringUtility;
}

// 包含 CfgActivity 等独立配置头文件
#include "Utility/Position.h"
#include "CfgActivity.h"
#include "CfgActivityMonster.h"
#include "CfgMap.h"

// 枚举定义
// CfgMapEvent 在此文件中有独立结构体定义(用于 config 加载), 与 CfgMapEvent.h 的 class 版本不同

// CfgMapEvent struct for CfgData internal use
#ifndef CFGMAPEVENT_STRUCT_DEFINED
#define CFGMAPEVENT_STRUCT_DEFINED
struct CfgMapEvent {
    int id;
    int trigger_id;
    bool bInitOpen;
    int trigger_type;
    std::vector<int> trigger_param;
    int event_type;
    std::string effect;
    int repeate;
    int activity_id;
    int plant_id;
    int count;
    int mapid;
    int nOpenTime;
    int nCloseTime;
    
    bool IsOpen() const { return bInitOpen && !m_bDone; }
    bool IsDone() const { return m_bDone; }
    void SetDone(bool done) { m_bDone = done; }
    
private:
    bool m_bDone = false;
};
#endif

// 枚举定义
enum class SERVER_TYPE {
    SVT_NORMAL,
    SVT_UNITE
};

// ==================== 结构体定义（简化版） ====================

struct AttrAddon {
    int index;
    int addon;
};

// Position is defined in Utility/Position.h (included above)

struct ItemData {
    int m_nId;
    int m_nClass;
    int m_nCount;
};

struct MemChrBag {
    int itemId;
    int itemClass;
    int itemCount;
    int bind;
};

// 通知 CExtCharBag.h 等文件这些类型已经定义
#define ITEM_DATA_DEFINED_HERE
#define MEM_CHR_BAG_DEFINED_HERE

struct MemChrJobBag {
    int id;
    int type;
    int count;
    int bind;
    int job;
    int time;
};

struct CfgItem {
    int id;
    std::string name;
    std::string desc;
    int type;
    int level;
    int grade;
    int job;
    int in_value;
    int out_value;
    int bind;
    int combine;
    int quality;
    std::string url;
    std::string drop_url;
    std::string effect;
    std::string use_method;
    int downgrade;
    int group_id;
    int cd_group;
    int overlay;
    int can_sell;
    int broadcast;
    int valid_time;
    int item_Grade;
    int CanDrop;
    int useBroadcast;
    int useInCarrier;
    int GongGaoId;
    int KunLingJingHua;
    int XinMoBag;
    int RongHeReceovery;
};

struct CfgItemGift {
    int id;
    int item;
    int type;
    int count;
    int bind;
    int job;
    int time;
};

struct CfgItemGiftRandom {
    int id;
    int item;
    int type;
    int count;
    int bind;
    int static_probability;
    int sum_probability;
    int job;
    int MinLevel;
    int MaxLevel;
};

struct CfgActivityNpc {
    int id;
    int activity_id;
    int npc_id;
    int count;
    std::vector<int> region_id;
};

struct CfgActivityPlant {
    int id;
    int activity_id;
    int plant_id;
    int count;
    int wave;
    int region_id;
    int whoplant;
    std::vector<Position> EnterPosVector;
    int life_time;
};

struct CfgActivityDrop {
    int id;
    int activity_id;
    int monster_min_level;
    int drop_group_id;
    int probability;
    int bind_type;
};

struct CfgActivityTrap {
    int id;
    int tid;
    int x;
    int y;
};

struct CfgBuff {
    int id;
    int groupid;
    int attack_type;
    int level;
    int beneficial;
    int special;
    int duration;
    int interval;
    std::vector<int> buffAttr;
    int isShow;
    int modify;
    int control;
    int hpValue;
    int hpPecent;
    int shieldValue;
    int shieldRatio;
    int effectType;
    int effectTimes;
    int canRemove;
    int canRecover;
    int angry;
    int battle;
};

struct CfgDungeon {
    int id;
    std::string name;
    int mapid;
    int x;
    int y;
    int type;
    int sequence;
    int group_id;
    int duration;
    int job;
    int level;
    int maxLevel;
    int vip;
    int player_num;
    int weekday;
    int start_hour;
    int end_hour;
    int daily_count;
    int last_id;
    int next_id;
    int player_buff;
    int star;
    int double_cost;
    int reward_time;
    ItemData costItem;
    std::vector<MemChrBag> rewardItem;
    std::vector<MemChrBag> rewardOnce;
    int Battle;
    int costMoney;
    int costGold;
    int buyTimes;
    int buyCost;
    int quickDoneCost;
    int quickDoneDrop;
    int needUpTowerStar;
    int summon_cost;
    int summon_boss;
    int rand_time;
    int task_id;
    int stay_position;
    std::vector<int> win_star;
    std::vector<MemChrBag> star_reward;
    std::vector<int> star_ratio;
    int TeQuan;
    int backMapId;
    int backX;
    int backY;
    int CanEnter;
    int BuffId;
};

struct CfgDungeonReward {
    int dungeonID;
    int exp;
    int money;
    int cash;
    int groupID;
};

struct CfgDungeonDrop {
    int type;
    int item_id;
    int item_class;
    int bind_type;
    int count;
    int weight;
    int probability;
};

struct CfgDungeonDropGroup {
    // 用于管理掉落组
};

struct CfgDungeonMonster {
    int id;
    int wave;
    std::list<int> mids;
    int x;
    int y;
    int count;
    int side;
    std::vector<Position> road;
    int delay;
    int times;
    int money;
    int life;
    std::vector<Position> randpos;
    int wait;
};

struct CfgDungeonPlant {
    int id;
    // 成员变量
};

struct CfgDungeonTrap {
    int id;
    // 成员变量
};

struct CfgDungeonNpc {
    int id;
    // 成员变量
};

struct CfgJob {
    int id;
    int job_task;
    int attack_attr;
    int base_skill;
};

struct CfgMovie {
    int id;
};

struct CfgLevelExp {
    int level;
    int64_t upgrade_exp;
    int64_t max_exp;
    int pet_exp;
    int mount_exp;
    int vicegeneral_id;
    int attr_point;
    int talent_point;
};

// CfgMap is defined in CfgMap.h (included above)

struct CfgMapMonster {
    int id;
    int mapid;
    int monsterid;
    int x;
    int y;
    int hide;
    int side;
    int boss;
    int Day;
};

struct CfgMapPlant {
    int mapid;
    int plantid;
    int x;
    int y;
};

struct CfgMapRegion {
    int id;
    int mapid;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    int type;
    int mId;
};

struct CfgMonster {
    int mid;
    int group_id;
    int level;
    int quality;
    int vAttr[50];
    int rand_count;
    std::vector<int> rand_types;
    int exp;
    int type;
    int revive_time;
    int skill_id;
    int unique_skill[10];
    int random_skill[10];
    int hpPercent;
    int ai;
    int broadcast;
    int drop_free;
    int boss_sign;
    int BossSocre;
    int TaskShare;
    int camp;
    int kill_point;
    int revive_skin;
    int PortalRatio;
    int PortalId;
    int PortalDuration;
    int prestige;
    int dust;
    int is_building;
    int PureDamage;
    int IfCurse;
    int Score;
    int IsShowOwner;
    int DiligenceType;
    int DiligenceValue;
    int64_t MonsterHp;
    int CanTunshi;
    int Mid2;
    int FamilyValue;
    int IsXinMoMonster;
    int QieGe;
    int Energy;
    MemChrBag cItem;
    int IsSunAndMoon;
    int corpse_time;
};

struct CfgMonsterAI {
    int id;
    int style;
    int target;
    int escape_hp;
    int view_range;
    int move_range;
    int rest_range;
    int rest_time_min;
    int rest_time_max;
    int rest_ratio;
    int run_distance;
    int run_range;
    int run_cd;
    int pursuit_range;
};

struct CfgMonsterAdjust {
    int mid;
    int adj_level;
    int level;
    int vAttr[50];
    int exp;
};
typedef CfgMonsterAdjust CfgAdjust;
typedef CfgMonsterAdjust CfgMonsterAdj;

struct CfgMonsterDropGroup {
    int group_id;
    int item_id;
    int item_class;
    int item_count;
    int bind_type;
    int weight;
    int probability;
    int cost_type;
    int cost_value;
    int limit_time;
    int daily_limit;
    int start_date;
    int end_date;
    int record;
    int festival_group;
};

struct CfgMonsterGroupDrop {
    int mid;
    int group_id;
    int probability;
    int begin_time;
    int end_time;
    int repeat;
    int job;
    int grow_level;
    int dropType;
    int bind_type;
    int hard;
    int quality;
    int world_event;
    int DropLimit;
};

struct CfgMonsterTaskDrop {
    int mid;
    int tid;
    int item;
    int probability;
};

struct AddAttribute {
    int m_nAddAttrType;
    int m_nAddAttrValue;
};

struct CfgEquipItem {
    // 成员变量
};

struct CfgGongGao {
    // 成员变量
};

struct CfgDungeonNpcCost {
    int ItemType;
    int ItemId;
    int ItemCount;
};

struct CfgPlantEvent {
    int EventId;
    int Probability;
};

struct TaskRequest {
    int param1;
    int param2;
    int param3;
};

struct TaskDrop {
    // 成员变量
};

struct RefreshMonster {
    int nCount;
    int BossId;
    int AliveTime;
    int GongGaoId;
};

struct RateItem {
    // 成员变量
};

struct BuffAttr {
    int attr;
    int ratio;
    int addon;
};

struct MonsterSkill {
    int maxHp;
    int minHp;
    int skillid;
};

struct CfgTencentGift {
    int nLevel;
    std::vector<MemChrBag> vRewards;
    std::vector<MemChrBag> vVipRewards;
};

struct CfgQQGift {
    int nIndex;
    int nType;
    std::vector<MemChrBag> vReward;
    int nCondition;
};

struct CfgTGPGift {
    std::string strPF;
    int nIndex;
    int nLevel;
    std::vector<MemChrBag> vReward;
};

struct ShouHuRefinishingCfg {
    std::list<ItemData> lCostList;
    std::list<AddAttribute> lAttrList;
    int SuitId;
};

struct WingEquipPolish {
    std::list<AddAttribute> lAttrList;
    std::list<ItemData> lCostList;
    int nConstMoney;
    int SuitId;
    int GongGaoId;
};

struct WinRefiningCfg {
    // 成员变量
};

struct GuiGuDaoRenCfg {
    int nNpcId;
    int nMaxCount;
    std::list<RefreshMonster> lRefreshMonster;
    std::vector<ItemData> vItemData;
    std::vector<MemChrBag> vItem;
    std::vector<int> vMapId;
};

struct CfgShiZhuang {
    int nId;
    int nType;
    std::vector<AddAttribute> vAttr;
    int nSuitId;
};

struct CfgShiZhuangLevel {
    int nType;
    int nLevel;
    int nLevelExp;
    int nNeedLevel;
    int nCostItem;
    int nGetExp;
    int nSmallCritRate;
    int nLargeCritRate;
    int nBroadcast;
    std::vector<AddAttribute> vAttr;
};

struct MonthlyChouJiangTable {
    // 成员变量
};

struct ActDropItem {
    int nMinLevel;
    int nMaxLevel;
    int nMapType;
    int nId;
    int nClass;
    int nCount;
    int nBind;
    int nStartTime;
    int nEndTime;
    int nProbability;
};

struct XunLeiCfg {
    // 成员变量
};

struct CfgGuardPrivilege {
    // 成员变量
};

struct CycleTowerTable {
    // 成员变量
};

struct PlatformDaTing {
    // 成员变量
};

struct MiniClientCfg {
    // 成员变量
};

struct PlatformVip {
    // 成员变量
};

struct ShangRenCfg {
    // 成员变量
};

struct SelectItemCfg {
    int nIndex;
    std::vector<MemChrBag> vItem;
};

struct PlatformRewardCfg {
    // 成员变量
};

struct DiligenceCfg {
    // 成员变量
};

struct ContributionCfg {
    // 成员变量
};

struct HoeCfg {
    int nId;
    int nNextId;
    int nDouble;
};

struct CMingGeTable {
    // 成员变量
};

struct CrossTowerCfg {
    // 成员变量
};

struct KunLingTable {
    // 成员变量
};

struct LuDaShiVip {
    int nIndex;
    int nType;
    std::vector<MemChrBag> Rewards;
    int nCondition;
    int nVipType;
    int nMaxCondition;
};

struct FlopDrawCfg {
    // 成员变量
};

struct SevenTaskTable {
    // 成员变量
};

struct MoFuSkillCfg {
    // 成员变量
};

struct ZhuNengCfg {
    // 成员变量
};

struct MoFuDuiHuan {
    // 成员变量
};

struct ChristmasDuiHuan {
    // 成员变量
};

struct BaoKuFuBen {
    // 成员变量
};

struct BaoKuRandom {
    // 成员变量
};

struct CfgChargeDungeon {
    int nDungeonId;
    int nChargeValue;
    int nMinKaiFuDay;
    int nMaxKaiFuDay;
};

struct GongMingCfg {
    // 成员变量
};

struct XinMoTable {
    // 成员变量
};

struct RefreshMonsterCfg {
    // 成员变量
};

struct EnergyCfg {
    // 成员变量
};

struct VipEquipPosLevelUp {
    // 成员变量
};

struct VipEQuipPosSuit {
    int nLevel;
};

struct ShiZhuLevelUp {
    // 成员变量
};

struct ClbAimCfg {
    // 成员变量
};

struct CVipClubLuckyDrop {
    // 成员变量
};

struct ZhanHunCfg {
    // 成员变量
};

struct LittleHelperCfg {
    // 成员变量
};

struct ShiQuCfg {
    // 成员变量
};

struct ZeroBuyPetCfg {
    // 成员变量
};

struct SunAndMoonCfg {
    // 成员变量
};

struct TreasureHunterCfg {
    // 成员变量
};

struct NationalDayTaskCfg {
    // 成员变量
};

struct NationalDayLevelCfg {
    // 成员变量
};

struct RongHeCfg {
    // 成员变量
};

struct EquipRongHe {
    // 成员变量
};

struct CfgNpc {
    int id;
    int npcid;
    std::vector<int> mapids;
    int x;
    int y;
    int func;
    int func_extra;
    std::vector<int> params;
    int unite_flag;
    std::vector<std::string> platforms;
    std::vector<CfgDungeonNpcCost> m_vNpcCost;
};

struct CfgNpcAirport {
    int id;
    int npcid;
    int player_kingdom;
    int kingdom_id;
    int map_id;
    int map_x;
    int map_y;
    int cost;
};

struct CfgChrShop {
    int Index;
    int ItemId;
    int ItemClass;
    int IsBind;
    int LimitCount;
    int Price;
    int ConstType;
    int QiQinglevel;
};

struct CfgPlant {
    int id;
    int type;
    int level;
    std::vector<CfgPlantEvent> Events;
    int EventMaxRate;
    int item_cost;
    int start_hour;
    int end_hour;
    int gather_time;
    int revive_time;
    int hide_time;
    int get_points;
    int boss_id;
    int BuffId;
    int revive_skin;
    int TaskId;
    std::vector<int> ItemVt;
};

struct CfgTask {
    int id;
    char name[256];
    int type;
    int group;
    int can_giveup;
    int pretask;
    int posttask;
    int main_order;
    int job;
    int level;
    int max_level;
    int kingdom;
    int start_npc;
    int end_npc;
    int dungeon;
    std::vector<MemChrBag> items_receive;
    int award_exp;
    int award_money;
    int gold;
    int dilong;
    int rongyu;
    int fuwen;
    int BossScore;
    std::vector<MemChrBag> award_item;
    std::vector<MemChrJobBag> award_optional;
    int condition;
    TaskRequest request;
    int GongXian;
    int JunTuanZiJin;
    int Double;
    int acScore;
    int quickDoneCost;
    int quality;
    int ratio;
    int dust;
    int activity_score;
    int talent_point;
    std::list<TaskDrop> drop_list;
    int DoubleGold;
    int BuffId;
};

struct CfgTrap {
    int id;
    int cd;
    int delay;
    int event_type;
    std::string effect;
    int item_cost;
    int life;
};

struct CfgLevelAttr {
    int level;
    int job;
    std::vector<AttrAddon> addonattr;
    std::vector<AttrAddon> addonPoint;
};

struct CfgGroupIcon {
    int nId;
    int bShowInCross;
    std::string platfrom;
};

struct CfgEquip {
    int m_nId;
    int m_nType;
    int m_nGrade;
    int m_nLevel;
    int m_nJob;
    int m_nQuality;
    int m_nSuitId;
    int m_nSuitId2;
    int m_nPrice;
    std::vector<AttrAddon> m_vAttrAddon;
    std::vector<AttrAddon> m_vElement;
    int m_DropLuck;
    int m_DropRate;
    int m_BackType;
    int m_BackValue;
    int m_nBroadcast;
    int m_backIndex;
    int m_nLimitStar;
    int m_BackGold;
    std::vector<AttrAddon> m_BaseAttr;
    int m_PolishLevel;
    int m_MoFuHuiShou;
    int m_SellDay;
    int m_GongMingLevel;
    int m_WingEquipRefiningLevel;
    int m_XinMoExp;
    int m_XinMoBag;
    int m_CanRongHe;
    int m_TeJieParam;
};

struct CfgItemGem {
    int m_nId;
    int m_nLayNum;
    int m_nPrice;
    // 其他成员
};

struct CfgItemCombi {
    // 成员变量
};

struct CfgPetData {
    int m_nPetId;
    int m_Quality;
    int m_vSkill[9];
    int m_vSkillOpen[9];
};

struct CfgSkillLevelUp {
    // 成员变量
};

struct CfgTaskCycle {
    // 成员变量
};

struct CfgFamily {
    int nLevel;
    int nExp;
    int nMaxMembers;
    int vPosition[3];
    int nMaxBossPoints;
};

struct CfgFamilyWar {
    // 成员变量
};

struct CfgFamilyLightExp {
    // 成员变量
};

struct CfgHorseRacingReward {
    // 成员变量
};

struct CfgCampWar {
    // 成员变量
};

struct CfgCityWar {
    // 成员变量
};

struct CfgHallOfFame {
    // 成员变量
};

struct CfgTitle {
    int nId;
    int nType;
    std::vector<int> vParams;
    std::string sPlatform;
    int nJob;
    int nSex;
    int nSpecial;
    int nPriority;
    std::vector<AttrAddon> vGetAttr;
    std::vector<AttrAddon> vDressAttr;
};

struct CfgDropRecord {
    int nIndex;
    int nItemId;
    int nItemClass;
    int nSpecial;
};

struct CfgDungeonScore {
    int nDungeonId;
    int nKillCount;
    MemChrBag Item;
};

struct CfgDungeonSummon {
    int nDungeon;
    int nIndex;
    int nGold;
    int nLimit;
    std::list<int> lMonsters;
    ItemData ConstItem;
    int FreeCount;
};

struct CfgGoldEgg {
    int nId;
    int nGroupId;
    int nOpenTimes;
    int nCostMoney;
    int nCostGold;
};

struct CfgLimitTime {
    int nIndex;
    int nType;
    int nTime;
};

struct CfgMysteryShop {
    int nId;
    int nType;
    MemChrBag item;
    int nCostType;
    int nPrice;
    int nRate;
    int nMinLevel;
    int nMaxLevel;
    int nBroad;
    ItemData exchange;
};

struct CfgBuyGift {
    int nIndex;
    int nGold;
    std::vector<MemChrBag> vGift;
    int nBroad;
};

struct CfgExchange {
    int nIndex;
    int nType;
    std::list<ItemData> vCost;
    std::vector<MemChrBag> vReward;
    int nLimit;
};

struct CfgMysteryGift {
    int nIndex;
    int nType;
    int nCondition;
    std::vector<MemChrBag> vItem;
    int nBroadId;
};

struct CfgDrawReward {
    int nIndex;
    int nType;
    int nRate;
    std::vector<MemChrBag> vItem;
    int nBroad;
    int nId;
};

struct CfgMapRoad {
    int nIndex;
    int nNextIndex;
    int nMapId;
    std::vector<Position> road;
};

struct CfgTrailer {
    int nId;
    int nAttrList;
    int TrailerQuality;
    int nTaskId;
    int PureDamage;
};

struct CfgMYSJReward {
    int nId;
    int nClass;
    int nCount;
    int nBind;
    int nWeight;
    int nShow;
    int nGroup;
    int nMin;
    int nMax;
    int nBroad;
    int nSpecial;
    int nIndex;
};

struct CfgMaintainCompensate {
    int nIndex;
    int nTime;
    std::vector<MemChrBag> vItems;
};

struct CfgWishReward {
    int nTime;
    std::vector<MemChrBag> vReward;
};

struct CfgBFZLEnterCost {
    // 成员变量
};

struct CfgBlacketMarket {
    // 成员变量
};

struct CfgSuperMember {
    int nGold;
    int nQQ;
    int nIcon;
};

struct CfgSouGouSkin {
    std::vector<MemChrBag> vReward;
    int nIcon;
};

struct CfgPetEquip {
    int nId;
    int nType;
    int nPrice;
    int nQuality;
    int nSuitId;
    std::vector<AttrAddon> vAttr;
    int nBroadcast;
    int bCanDrop;
    int nGrade;
    std::vector<AttrAddon> vOwnerAttr;
    int nNeedStar;
};

struct CfgWeiXingGift {
    std::string strPlatform;
    int nIconId;
    std::vector<MemChrBag> vReward;
};

struct CfgAdultGift {
    std::string strPlatform;
    std::vector<MemChrBag> vReward;
    int nIconId;
};

struct CfgTencentSevenDayLogin {
    int nDays;
    std::vector<MemChrBag> vReward;
};

struct CfgFamilyDungeon {
    int nHard;
    int nDungeonId;
    int nFamilyLevel;
    int nCostMoney;
    int nBaoKuFuBen;
    int nNpcId;
    int X;
    int Y;
    int nTime;
};

struct cfgGoblinTableData {
    // 成员变量
};

struct cfgSpecialMonsterTable {
    // 成员变量
};

struct cfgEquipUpPhaseTable {
    // 成员变量
};

struct CfgTouZi {
    // 成员变量
};

struct CfgSearchBack {
    int nId;
    int nType;
    int nTimes;
    int nSubType;
    int nOpenType;
    int nMinLevel;
    int nMaxLevel;
    int nGoldCost;
    std::vector<MemChrBag> vFreeReward;
    std::vector<MemChrBag> vItemReward;
};

struct CfgEquipBless {
    // 成员变量
};

struct CfgSysMail {
    // 成员变量
};

struct BossInfo {
    // 成员变量
};

struct FaBaoTable {
    // 成员变量
};

struct CfgQuestions {
    // 成员变量
};

struct CfgGameShop {
    int ItemClass;
    int ItemId;
    int ShopType;
    // 其他成员
};

struct CQiFuTable {
    // 成员变量
};

struct VipTable {
    // 成员变量
};

struct CfgSignReward {
    signed char count;
    std::vector<MemChrBag> ItemVector;
};

struct CfgOnlineReward {
    signed char Id;
    int NeedTime;
    std::vector<MemChrBag> ItemVector;
};

struct CfgSevenLoginRewrad {
    signed char Day;
    std::vector<CfgEquipItem> ItemVector;
};

struct CfgLevelGift {
    int Index;
    int Level;
    std::vector<MemChrBag> ItemVector;
};

struct CfgWeekOnlineReward {
    int Week;
    int Cash;
    int UpperLimit;
};

struct CfgOffLineExp {
    int Level;
    int MinuteExp;
    int TwoTimes;
    int ThreeTimes;
    int BaseExp;
};

struct VipCardCfg {
    int VipCardId;
    int AddVipTime;
    int NeedGold;
    std::list<AddAttribute> AddAttr;
    int VipLevel;
    int ReNeedGold;
    int Money;
    int TianShuJinHua;
    int WeiWang;
};

struct FunctionOpenCfg {
    int Type;
    int TaskId;
    int Level;
};

struct CfgPlantEventEffect {
    int EventId;
    int EventType;
    std::string EventEffect;
    int GongGaoId;
};

struct CfgEquipExchange {
    signed char Level;
    int Money;
    ItemData CostItem;
};

struct CfgShouChong {
    int nIndex;
    int nGold;
    std::vector<CfgEquipItem> vReward;
    int nBroadcast;
};

class CfgFestivalActivity;
struct CfgChangeItemCfg;
struct CfgFestivalMapPos;
class CfgFestivalActivityTable;

struct NewServerFavorable {
    signed char Index;
    std::vector<CfgEquipItem> ItemVector;
    int NeedGold;
    std::list<CfgGongGao> GongGaoInfo;
    int nLimitTime;
};

struct CfgEverydayChongZhi {
    signed char Index;
    signed char Type;
    int NeedGold;
    std::vector<CfgEquipItem> ItemVector;
    std::list<CfgGongGao> GongGaoInfo;
};

struct CfgKaiFuHuoDongData {
    int Index;
    int Type;
    int Level;
    int StartDay;
    int EndDay;
    int Conditions;
    std::vector<MemChrBag> ItemVector;
    std::string ItemString;
    int PetAttr;
    int GongGaoId;
    int Limit;
    int PetEffect;
    int Conditions1;
    std::string FlopString;
};

struct CfgHuoYueDu {
    int Index;
    int Type;
    int Effect;
    int Count;
    int Gold;
    int AddHuoYueDu;
    bool IsCanSec;
};

struct CfgHuoYueDuReward {
    signed char Id;
    int NeedHuoYueDu;
    std::vector<MemChrBag> Items;
};

struct CfgAppendAttr {
    int Id;
    int Job;
    std::list<AddAttribute> AttrList;
};

struct SpecialTreasureMapRandCfg {
    int nItemId;
    int nType;
    std::vector<MemChrBag> vGetItems;
    int nRate;
    MemChrBag vShowItem;
    int nGongGaoId;
};

struct CfgFamilyTask {
    // 成员变量
};

struct FamilyTaskReward {
    int Index;
    int NeedCount;
    std::vector<MemChrBag> Rewards;
};

struct CfgTotalChongZhi {
    signed char Index;
    int NeedGold;
    std::vector<CfgEquipItem> ItemVector;
    std::list<CfgGongGao> GongGaoInfo;
    int NewServerday;
};

struct CfgMonsterAddAttr {
    int WorldBossLevelMin;
    int WorldBossLevelMax;
    std::vector<AttrAddon> AttrVector;
};

struct DaZheQuan {
    int Index;
    int CurrencyType;
    int CurrencyValues;
    std::vector<MemChrBag> Items;
};

struct Wan360Reward {
    int Index;
    std::vector<MemChrBag> Items;
};

struct CfgVplan {
    // 成员变量
};

struct Speed360Reward {
    // 成员变量
};

struct GroupMonster {
    // 成员变量
};

struct BuyFaBaoResCfg {
    // 成员变量
};

struct XingMaiCfg {
    // 成员变量
};

struct CfgXingMaiSlot {
    // 成员变量
};

struct TianLingCfg {
    // 成员变量
};

struct DamnationCfg {
    // 成员变量
};

struct ScoreShopCfg {
    // 成员变量
};

struct ChouJiangTable {
    // 成员变量
};

struct CLibraryTable {
    // 成员变量
};

struct TreasureMapTabale {
    // 成员变量
};

struct BackEquipTask {
    // 成员变量
};

struct Param2 {
    int nParam1;
    int nParam2;
};

struct SpeciaEquipCfg {
    // 成员变量
};

struct SpeciaSkillDistance {
    // 成员变量
};

struct LuckDropTable {
    // 成员变量
};

struct LevelRefinCfg {
    // 成员变量
};

struct CfgBossFirstKilled {
    // 成员变量
};

struct UltimateChallengeCfg {
    // 成员变量
};

struct CfgCachet {
    int nLevel;
    int64_t nNeedCacht;
    // 其他成员
};

struct BossKilledReward {
    // 成员变量
};

struct SystemOpenGift {
    // 成员变量
};

struct SpecialMap {
    // 成员变量
};

struct TestServerReward {
    // 成员变量
};

struct GuWuCfg {
    // 成员变量
};

struct FunctionOpenMail {
    // 成员变量
};

struct XiangYaoTaskCfg {
    int TaskId;
    int MinLevel;
    int MaxLevel;
    int Rate;
    int Star;
};

struct ShenWeiTaskCfg {
    // 成员变量
};

struct CfgMobilePhoneGift {
    std::string strPlatfrom;
    std::vector<MemChrBag> vItem;
    int nIcon;
};

struct CfgMiniClient {
    std::string strPlatfrom;
    int nIconDownload;
    int nIconLogin;
    std::vector<MemChrBag> vReward;
};

struct CfgWuHunShop {
    int Index;
    MemChrBag Item;
    int Rate;
    int Const;
};

struct WuHunItem {
    int nId;
    int nLevel;
    int nType;
    int nQuality;
    int nNeedQuality;
    std::list<AddAttribute> lAttrList;
    int nTalentId;
    int nTalentLevel;
    int overlay;
    int nDressLevel;
};

struct CreateWuHun {
    int nId;
    std::list<ItemData> ConstItem;
    std::list<RateItem> GetItemRate;
    std::list<RateItem> GetItemRate2;
    ItemData SpecialCost;
};

struct BossDistribution {
    // 成员变量
};

struct SpecialBossMapCfg {
    int MapId;
    ItemData ConstItem;
    int StartCD;
    int ContinuedTime;
};

struct DuiHuanLimit {
    int ShengYaoBi;
    int ConstGold;
    int ConstCurrency;
    int Limit;
};

struct JewelPavilionCfg {
    int nDay;
    int nIndex;
    MemChrBag Item;
    int nPrice;
};

struct SuperTeHuiCfg {
    int nIndex;
    int nNeedVip;
    std::vector<MemChrBag> Items;
    int nPrice;
    int nGongGaoId;
};

struct GoblinCfg {
    int UpAttr;
    int SuitId;
    int ConstCurr;
};

// ==================== 缺失类型的空定义 ====================
struct MemJobItem {};
struct CfgMountEquip {};
struct CfgSkillTable {};
struct CfgTalentTable {};
struct CfgMonsterRandTable {};
struct CfgWingTable {};
struct CfgCarrierTable {};
struct CfgJueWeiTable {};
struct CfgEquipBoxTable {};
struct CfgActivityTaskTable {};
struct CfgYYGameApp {};
struct CfgYYVip {};
struct CfgYYSuperBuff {};
struct CfgSgGameApp {};
struct CfgEquipTable {};
struct CfgItemGemTable {};
struct CfgItemCombiTable {};
struct CfgPetTable {};
struct CfgSkillLevelUpTable {};
struct CfgTaskCycleTable {};
struct CfgFamilyTable {};
struct CfgFamilyWarTable {};
struct CfgFamilyLightExpTable {};
struct CfgHorseRacingRewardTable {};
struct CfgCampWarTable {};
struct CfgCityWarTable {};
struct CfgHallOfFameTable {};
struct CfgTitleTable {};
struct CfgDungeonScoreTable {};
struct CfgGoldEggTable {};
struct CfgLimitTimeTable {};
struct CfgMysteryShopTable {};
struct CfgExchangeTable {};
struct CfgMysteryGiftTable {};
struct CfgDrawTable {};
struct CfgMapRoadTable {};
struct CfgTrailerTable {};
struct CfgMYSJRewardTable {};
struct CfgMaintainCompensateTable {};
struct CfgWishRewardTable {};
struct CfgBFZLEnterCostTable {};
struct CfgBlacketMarketTable {};
struct CfgPetEquipTable {};
struct CfgWeiXinTable {};
struct CfgAdultGiftTable {};
struct CfgTencentTable {};
struct CfgFamilyDungeonTable {};
struct CfgTouZiTable {};
struct CfgSearchBackTable {};
struct CfgEquipBlessTable {};
struct CfgShouChongTable {};
struct CfgShiZhuangTable {};
struct CfgGuardPrivilegeTable {};
struct CfgBeastShrineTable {};
struct CfgOutLinkFestivalTable {};
struct ChargeDungeonCfg {};
struct PkDropRate {};
struct BaoKuRandomMap {};
struct CfgFestivalActivityTable {};

// ==================== CfgData 类定义 ====================

class CfgData {
public:
    // 类型别名（供所有方法声明使用）
    typedef std::vector<CfgItemGift> CfgItemGiftVector;
    typedef std::vector<CfgItemGiftRandom> CfgItemGiftRandomVector;
    typedef std::vector<CfgMapMonster> CfgMapMonsterVector;
    typedef std::vector<CfgMapPlant> CfgMapPlantVector;
    typedef std::vector<CfgMapRegion> CfgMapRegionVector;
    typedef std::vector<CfgMonsterDropGroup> CfgMonsterDropGroupVector;
    typedef std::vector<CfgMonsterGroupDrop> CfgMonsterGroupDropVector;
    typedef std::vector<CfgMonsterTaskDrop> CfgMonsterTaskDropVector;
    typedef std::list<CfgMapEvent> CfgMapEventListT;
    typedef std::map<int, CfgItem*> CfgItemTable;
    typedef std::vector<BuffAttr> BuffAttrVector;
    typedef std::vector<int> CfgInt32Vector;
    typedef std::vector<AttrAddon> AttrAddonVector;
    typedef std::vector<MemChrBag> CfgMemChrBagVector;
    typedef std::vector<MemChrJobBag> MemChrJobBagVector;
    typedef std::vector<std::vector<int>> CfgInt32VtVector;
    typedef std::map<int, MemJobItem> MemJobItemTable;

    // 单例
    static CfgData* instance();
    static CfgData* GetInstance();
    
    // 初始化
    bool init(int32_t line, int32_t debug);
    void reload();
    
    // Getter 方法
    int32_t GetMapMonsterId();
    
    CfgActivity* getActivity(int32_t id);
    const CfgMapEventListT* getActivityEvents(int32_t activity_id, int32_t nMapId);
    CfgActivityMonster* getActivityMonster(int32_t id);
    CfgActivityNpc* getActivityNpc(int32_t id);
    CfgActivityPlant* getActivityPlant(int32_t Id);
    CfgActivityTrap* getActivityTrap(int32_t id);
    
    CfgBuff* getBuff(int32_t id);
    
    CfgDungeon* getDungeon(int32_t id);
    const CfgDungeon* GetUpTowerDungeon(int32_t nFloor);
    const CfgMapEventListT* getDungeonEvent(int32_t dungonid);
    CfgDungeonMonster* getDungeonMonster(int32_t id);
    CfgDungeonPlant* getDungeonPlant(int32_t id);
    CfgDungeonTrap* getDungeonTrap(int32_t id);
    CfgDungeonNpc* getDungeonNpc(int32_t id);
    
    const CfgEquip* getEquip(int32_t id);
    CfgChrShop* getChrShop(int32_t Index);
    CfgItemTable* getAllItem();
    CfgItem* getItem(int32_t id);
    CfgItemGiftVector* getItemGift(int32_t id);
    CfgItemGiftRandomVector* getItemGiftRandom(int32_t id);
    
    CfgJob* getJob(int32_t id);
    bool getMovie(int32_t id);
    
    CfgMap* getMap(int32_t id);
    CfgMapMonsterVector* getMonstersOnMap(int32_t mapid);
    CfgMapMonster* GetMapMonsterInfo(int32_t id);
    CfgMapPlantVector* getPlantOnMap(int32_t mapid);
    CfgMapPlant* GetMapPlantInfo(int32_t id);
    CfgMapRegion* getMapRegion(int32_t id);
    CfgMapRegionVector* getRegionOnMap(int32_t mapid);
    
    CfgMonster* getMonster(int32_t mid);
    CfgMonsterAI* GetMonsterAI(int32_t nId);
    CfgMonsterAdjust* GetMonsterAdjust(int32_t mid, int32_t adj_level, bool bAutoLow);
    CfgAdjust* GetCfgAdjust(int32_t mid, int32_t adj_level, bool bAutoLow);
    bool isMonsterBroadcast(int32_t mid);
    
    CfgDungeonReward* getDungeonReward(int32_t dungeonID);
    const CfgDungeonDrop* randDungeonDrop(int32_t dungeonID);
    
    CfgMonsterDropGroupVector* getMonsterDropGroup(int32_t group_id);
    CfgMonsterGroupDropVector* getMonsterGroupDrop(int32_t mid);
    CfgMonsterTaskDropVector* getMonsterTaskDrop(int32_t mid);
    
    CfgNpc* getNpc(int32_t npcid);
    CfgNpcAirport* getNpcAirport(int32_t id);
    
    CfgPlant* getPlant(int32_t id);
    
    CfgTask* getTask(int32_t id);
    int8_t GetTaskType(int32_t id);
    
    CfgTrap* getTrap(int32_t id);
    
    CfgLevelExp* getUpgradeExp(int32_t level);
    int64_t getNeedLevelExp(int32_t level);
    int64_t getMaxExp(int32_t level);
    int32_t GetPetExp(int32_t level);
    int32_t GetTalentPoint(int32_t level);
    int32_t GetAttrPoint(int32_t level);
    
    CfgLevelAttr* getLevelAttr(int32_t job, int32_t level);
    int32_t getBaseJob(int32_t job);
    
    // 服务器配置相关
    int32_t getServerStartTime(SERVER_TYPE nType);
    int32_t getServerStartDayZeroTime(SERVER_TYPE nType);
    int32_t getServerDiffDay(SERVER_TYPE nType);
    void updateServerStartTime(int32_t kaiFuTime);
    int32_t getServerType() { return m_serverType; }
    
    // Init 方法（声明部分）
    void InitGroupIconTable();
    bool IsShowIcon(int32_t nIconId, const std::string* platform);
    void InitFamilyRewardTable();
    FamilyTaskReward* GetFamilyReward(int32_t Index);
    void InitAppendAttrTable();
    void InitHuoYueDuTable();
    CfgHuoYueDu* GetHuoYueDuCfg(int32_t Index);
    CfgHuoYueDuReward* GetHuoYueDuReward(int32_t Index);
    void InitKaiFuHuoDongTable();
    void InitDropRecordTable();
    const CfgDropRecord* GetDropRecord(int32_t nIndex);
    void InitDungeonScoreTable();
    void InitDungeonSummon();
    const CfgDungeonSummon* GetDungeonSummon(int32_t nDungeon, int32_t nIndex);
    void InitSpecialTreasureMapRandTable();
    void InitMonsterAddAttrTable();
    AttrAddonVector* GetAddMonsterAttrs(int32_t Mid, int32_t WorldLevel);
    void InitDaZheQuanTable();
    DaZheQuan* GetDaZheQuanCfg(int32_t Index);
    void Init360RewardTable();
    void Init360RewardTypeTable();
    Wan360Reward* Get360RewardCfg(int32_t Index);
    int32_t Get360RewardIcon(const std::string* platform);
    void InitTotalChongZhiTable();
    CfgTotalChongZhi* GetTotalChongZhiCfg(int8_t Index);
    void InitEveryDayChongZhi();
    CfgEverydayChongZhi* GetEveryDayChongZhiCfg(int8_t nType, int8_t Index);
    void InitNewServerFavorable();
    NewServerFavorable* GetNewServerFavorable(int8_t Index);
    void InitShouChongLiBao();
    void InitMoHuaHuanYiTable();
    CfgEquipExchange* GetEquipExchange(int8_t Level);
    void InitZiYuanZhaoHuiTable();
    void InitPlantEventTable();
    CfgPlantEventEffect* GetPlantEvent(int32_t EventId);
    void InitFunctionOpenTable();
    FunctionOpenCfg* GetOpenFunctionCfg(int32_t FunctionId);
    void InitVipCardTable();
    VipCardCfg* GetVipCardCfg(int8_t VipType);
    void InitOffLineExpTable();
    CfgOffLineExp* GetOfflineExpCfg(int32_t Level);
    void InitWeekOnlineReward();
    CfgWeekOnlineReward* GetWeekOnlineReward(int32_t Week);
    void InitLevelGift();
    CfgLevelGift* GetLevelGiftCfg(int8_t Index);
    void InitSevenLoginReward();
    CfgSevenLoginRewrad* GetSevenLoginRewardCfg(int8_t Day);
    void InitOnLimeReward();
    CfgOnlineReward* GetOnlineRewardCfg(int8_t id);
    void fetchSignReward();
    CfgMemChrBagVector* GetSignReward(int8_t SiginCount);
    
    void InitQQZoneRewardTable();
    void InitQQGameRewardTable();
    void InitYellowRewardTable();
    void InitTencentSevenDayLoginTable();
    void InitYellowDailyRewardTable();
    void InitYellowLevelRewardTable();
    void InitBlueRewardTable();
    void InitBlueDailyRewardTable();
    void InitBlueLevelRewardTable();
    void InitTGPRewardTable();
    void InitTGPDailyRewardTable();
    void InitTGPLevelRewardTable();
    void InitFamilyDungeonTable();
    
    const std::map<int, CfgActivity>* GetAllActivity() { return &m_activities; }
    void fetchActivity();
    void fetchBuff();
    void fetchDungeon();
    void fetchItem(bool bSend);
    void fetchServerConfig(int32_t line);
    void fetchDungeonEvent();
    void fetchDungeonMonster();
    void fetchDungeonPlant();
    void fetchDungeonTrap();
    void fetchDungeonNpc();
    void fetchMap();
    void fetchMapMonster();
    void fetchMapPlant();
    void fetchMapRegion();
    void fetchMonster();
    void fetchMonsterAI();
    void fetchMonsterAdjustTable();
    void fetchMonsterDropGroup();
    void fetchMonsterGroupDrop();
    void fetchMonsterTaskDrop();
    void fetchNpc();
    void fetchPlant();
    void fetchTask();
    void fetchTrap();
    void fetchLevelExp();
    void fetchLevelAttr();
    void fetchJob();
    void fetchChrShop();
    void fetchMovie();
    
    void InitActiveSkillTable();
    void InitPassiveSkillTable();
    void InitTrigSkillTable();
    void InitTalentTable();
    void InitTalentPageTable();
    void InitFamilySkillTable();
    void InitTalentActiveTable();
    void InitEquipTable();
    void InitEquipUpStarTable();
    void InitEquipSuitTable();
    void InitEquipDecomposeTable();
    void InitEquipUpPosTable();
    void InitEquipPosSuitTable();
    void InitEquipBoxTable();
    void InitItemGemTable();
    void InitGemOpenHoleTable();
    void InitGemLevelUpTable();
    void InitGemSuitTable();
    void InitItemCombiTable();
    void InitMonsterRandTable();
    void InitCarrierTable();
    void InitCarrierAttrTable();
    void InitJueWeiTable();
    void InitPetTable();
    void InitPetAttrTable();
    void InitPetUpStarTable();
    void InitSkillLevelUpTable();
    void InitTaskCycleStarTable();
    void InitTaskCycleRewardTable();
    void InitFamilyTable();
    void InitFamilyWarJoinRewardTable();
    void InitFamilyWarReliveTable();
    void InitFamilyLightExpTable();
    void InitHorseRacingRewardTable();
    void InitCampWarRewardTable();
    void InitCampWarContKillTable();
    void InitCityWarContRewardTable();
    void InitHallOfFameRewardTable();
    void InitTitleTable();
    void InitGoldEggTable();
    void InitLimitTimeTable();
    void InitMysteryShopTable();
    void InitBuyGiftTable();
    void InitExchangeTable();
    void InitMysteryGiftTable();
    void InitDrawTable();
    void InitMapRoadTable();
    void InitTrailerTable();
    void InitMYSJRewardTable();
    void InitMaintainCompensateTable();
    void InitWishRewardTable();
    void InitBFZLEnterCostTable();
    void InitBlacketMarketTable();
    void InitLevelChatTimesTable();
    void InitSuperMemberTable();
    void InitSouGouSkinTable();
    void InitPetEquipTable();
    void InitWeiXinTable();
    void InitAdultGiftTable();
    void InitMobilePhoneGiftTable();
    void InitMiniClientTable();
    void InitWuHunShopTable();
    void InitWuHunItemTable();
    void InitWuHunCreateTable();
    void InitBossDistribution();
    void InitSpecialBossMapCfgMap();
    void InitDuiHuanLimitTable();
    void InitSuperTeHuiTable();
    void InitJewelPavilionTable();
    void InitGoblinTable();
    void InitShouHuRefining();
    void InitWingEquipPolish();
    void InitGuiGuDaoRenTable();
    void InitShiZhuangTable();
    void InitShiZhuangLevelTable();
    void InitMonthlyChouJiangTable();
    void InitActDropTable();
    void InitSevenTaskTable();
    void InitOutLinkFestivalTable();
    void InitYYDaTing();
    void InitYYVip();
    void InitSouGouDaTing();
    void InitXunLeiTable();
    void InitCycleTowerTable();
    void InitPlatformDaTingMap();
    void InitMiniClientMap();
    void InitPlatformVipMap();
    void InitShangRenCfgMap();
    void InitSelectItemCfgMap();
    void InitGuardPrivilegeTable();
    void InitPlatformReward();
    void InitDiligenceCfgMap();
    void InitFamilyDonateTable();
    void InitFamilyMedalTable();
    void InitHoeTable();
    void InitMingGeTable();
    void InitCrossTowerCfgMap();
    void InitKunLingTable();
    void InitLaDaShiHuiYuan();
    void InitFlopTable();
    void InitMoFuTable();
    void InitChristmasDuiHuanTable();
    void InitBaoKuFuBen();
    void InitChargeDungeon();
    void InitShenYaoPosTable();
    void InitShenYaoPosSuitTable();
    void InitTongTianChiRanTable();
    void InitXinMoTable();
    void InitGameTable();
    void InitRefreshMonsterCfgListMap();
    void InitLittleHelperCfg();
    void InitShiZhuLevelUp();
    void InitNationalDayTask();
    void InitRongHeCfg();
    void InitFestivalActivityTable();
    CfgFestivalActivityTable* GetFestivalActivityTable();
    
    // 辅助方法
    int32_t getOverlay(int32_t nId, int8_t nClass);
    int32_t getOutValue(int32_t nId, int8_t nClass);
    bool dropBroadcast(int32_t nId, int8_t nClass);
    bool canSell(int32_t nId, int8_t nClass);
    int32_t InitXuWuTask(int32_t Level, int32_t Times);
    int32_t InitXinMoTask(int32_t Level, int32_t Times);
    int32_t RandPdbfTask(int32_t Level);
    int32_t GetMonsterReviveTime(int32_t Time, int32_t BossId);
    int32_t GetChatTimes(int32_t nLevel);
    
    const CfgSuperMember* GetSuperMember(const std::string* platform);
    const CfgSouGouSkin* GetSouGouSkin(const std::string* platform);
    const CfgMiniClient* GetMiniClient(const std::string* platform);
    const CfgMobilePhoneGift* GetMobilePhoneGift(const std::string* platform);
    CfgYYGameApp* GetYYGameApp(int32_t nIndex);
    LuDaShiVip* GetLaDaShiHuiYuan(int32_t nIndex);
    CfgYYVip* GetYYVip(int32_t nIndex);
    CfgSgGameApp* GetSoGouGameApp(int32_t nIndex);
    const CfgYYSuperBuff* GetSuperBuff(int32_t SuperLevel);
    const CfgYYSuperBuff* Get37wanSuperBuff(int32_t SuperLevel);
    
    PlatformDaTing* GetGetPlatformDaTingCfg(std::string* p_Platform, int32_t nId);
    MiniClientCfg* GetMiniClientCfg(std::string* p_Platform, int32_t nId);
    PlatformVip* GetGetPlatformVipCfg(std::string* p_Platform, int32_t nId);
    PlatformRewardCfg* GetGetPlatformRewardCfg(std::string* p_Platform, int32_t nId);
    DiligenceCfg* GetDiligenceCfg(int32_t nType);
    ContributionCfg* GettFamilyDonateCfg(int32_t nId);
    ShangRenCfg* GetShangRenCfg(int32_t nId);
    SelectItemCfg* GetSelectItemCfg(int32_t nId);
    XunLeiCfg* GetXunLeiCfg(int32_t nIndex);
    CrossTowerCfg* GetCrossTowerCfg(int32_t MapId);
    MoFuSkillCfg* GetMoFuSkillCfg(int32_t nLevel);
    ZhuNengCfg* GetZhuNengCfg(int32_t nType, int32_t nLevel);
    MoFuDuiHuan* GetMoFuDuiHuanlCfg(int32_t nId);
    ChristmasDuiHuan* GetChristmasDuiHuanCfg(int32_t nId);
    int32_t GetTongTianChiReward(int32_t nId);
    int32_t GetChargeDungeonId(int32_t nId, int32_t Todaycharge);
    BaoKuFuBen* GetBaoKuFuBen(int32_t FuBenId);
    BaoKuRandomMap* GetBaoKuRandomMap();
    const TreasureHunterCfg* GetTreasureHunterCfg(int32_t nId);
    TianLingCfg* GetTianLingCfg(int32_t TianLingLevel);
    const SunAndMoonCfg* GetSunAndMoonCfg(int32_t nLevel);
    const ZhanHunCfg* GetZhanHunCfg(int32_t nLevel);
    const VipEquipPosLevelUp* GetVipEquipPosLevelUp(int32_t nSlot, int32_t nLevel);
    const VipEQuipPosSuit* GetVipEQuipPosSuit(int32_t nLevel);
    const CVipClubLuckyDrop* GetCVipClubLuckyDrop(int32_t nCondition);
    const ZeroBuyPetCfg* GetZeroBuyPetCfg();
    const ShiQuCfg* GetShiQuCfg(int32_t nId);
    const LittleHelperCfg* GetLittleHelperCfg(int32_t nId);
    const ShiZhuLevelUp* GetShiZhuLevelUp(int32_t nId, int32_t nLevel);
    const NationalDayTaskCfg* GetNationalDayTaskCfg(int32_t nType, int32_t nId);
    const NationalDayLevelCfg* GetNationalDayLevelCfg(int32_t nLevel);
    const EquipRongHe* GetEquipRongHe(int32_t ItemId);
    const RongHeCfg* GetRongHeCfg(int32_t nId);
    
private:
    friend class Answer::Singleton<CfgData>;
    CfgData();
    ~CfgData();
    
    // 辅助解析方法
    void sendNewItems(const CfgItemTable* items);
    void parseMonsterSkill(int32_t id, MonsterSkill (*vSkill)[10], const std::string* strSkill);
    TaskRequest parseTaskCondition(int32_t id, int32_t condition, const std::string* strRequest);
    void parseTaskDrop(int32_t id, const std::string* strDrop);
    std::vector<Position>* paresPosition(const std::string* strPos);
    Param2 paraseParam2(const std::string* str);
    CfgInt32Vector* paraseInt32Vector(const std::string* str, const std::string* path, int32_t size);
    AttrAddonVector* paraseAttrAddon(const std::string* addonAttr, int32_t nIndex, const std::string* path);
    BuffAttrVector* paraseBuffAttr(const std::string* str);
    void parseAddAttribues(const std::string* addonAttr, int32_t nIndex, const std::string* path);
    void paraseTalentAddon(const std::string* str, int32_t nIndex, const std::string* path);
    void paraseParam2List(const std::string* str, int32_t nIndex, const std::string* path);
    void paraseInt32List(std::list<int> *result, const std::string *str, int32_t size, const std::string *path);
    CfgInt32Vector* paraseInt32Vector2(const std::string* str, const std::string* path, int32_t size);
    CfgInt32VtVector* paraseInt32VtVector(const std::string* str, const std::string* path);
    void parseTaskItemJobString(int32_t id, const std::string* strItems);
    MemJobItemTable* parseGambleEquip(int32_t id, const std::string* strItems);
    void parseEquipItemString(int32_t nIndex, const std::string* strItems);
    void parseGongGaoString(int32_t nIndex, const std::string* strItems);
    
    // 成员变量
    int32_t m_debug;
    int32_t m_serverType;
    int32_t m_MapMonsterId;
    int32_t m_kaiFuTime;
    int32_t m_heFuTime;
    
    // 所有配置表容器
    std::list<CfgMapEvent> m_emptyEvents;
    std::map<int, CfgActivity> m_activities;
    std::map<int, std::map<int, std::list<CfgMapEvent>>> m_activityEvents;
    std::map<int, CfgActivityMonster> m_activityMonsters;
    std::map<int, CfgActivityNpc> m_activityNpcs;
    std::map<int, CfgActivityPlant> m_activityPlants;
    std::map<int, CfgActivityDrop> m_activityDrops;
    std::map<int, CfgActivityTrap> m_activityTraps;
    std::map<int, CfgBuff> m_buffs;
    std::map<int, CfgDungeon> m_dungeons;
    std::map<int, CfgDungeon> m_mUpTowerDungeon;
    std::map<int, CfgDungeonReward> m_dungeonReward;
    std::map<int, CfgDungeonDropGroup> m_dungeonDropGroup;
    std::map<int, std::list<CfgMapEvent>> m_dungeonEvents;
    std::map<int, CfgDungeonMonster> m_dungeonMonsters;
    std::map<int, CfgDungeonPlant> m_dungeonPlants;
    std::map<int, CfgDungeonTrap> m_dungeonTraps;
    std::map<int, CfgDungeonNpc> m_dungeonNpcs;
    std::map<int, CfgItem*> m_items;
    std::mutex m_itemsLock;
    std::map<int, std::vector<CfgItemGift>*> m_itemGifts;
    std::mutex m_itemGiftsLock;
    std::map<int, std::vector<CfgItemGiftRandom>*> m_itemGiftRandoms;
    std::mutex m_itemGiftRandomsLock;
    std::map<int, CfgMountEquip> m_mountEquip;
    std::map<int, CfgJob> m_jobs;
    std::map<int, CfgMovie> m_movie;
    std::map<int, CfgLevelExp> m_levelExps;
    std::map<int, CfgMap> m_maps;
    std::map<int, std::vector<CfgMapMonster>> m_mapMonsters;
    std::map<int, CfgMapMonster> m_CfgMapMonsters;
    std::map<int, std::vector<CfgMapPlant>> m_mapPlants;
    std::map<int, CfgMapPlant> m_mMapPlants;
    std::map<int, CfgMapRegion> m_mapRegions;
    std::map<int, std::vector<CfgMapRegion>> m_mapRegionsByMapId;
    std::map<int, CfgMonster> m_monsters;
    std::map<int, CfgMonsterAI> m_mMonsterAI;
    std::map<std::pair<int, int>, CfgMonsterAdjust> m_mMonsterAdjust;
    std::vector<int> m_monsterBroadcasts;
    std::map<int, std::vector<CfgMonsterDropGroup>> m_monsterDropGroups;
    std::map<int, std::vector<CfgMonsterGroupDrop>> m_monsterGroupDrops;
    std::map<int, std::vector<CfgMonsterTaskDrop>> m_monsterTaskDrops;
    std::map<int, CfgNpc> m_npcs;
    std::map<int, CfgNpcAirport> m_npcAirports;
    std::map<int, CfgChrShop> m_chrShops;
    std::map<int, CfgPlant> m_plants;
    std::map<int, CfgTask> m_tasks;
    std::map<int, CfgTrap> m_traps;
    std::map<int, CfgLevelAttr> m_levelAttrs;
    std::map<int, CfgGroupIcon> m_cfgGroupIcons;
    CfgSkillTable m_cfgSkillTable;
    CfgTalentTable m_cfgTalentTable;
    CfgMonsterRandTable m_cfgMonsterRandTable;
    CfgWingTable m_cfgWing;
    CfgCarrierTable m_cfgCarrierTable;
    CfgJueWeiTable m_cfgJueWeiTable;
    CfgEquipBoxTable m_cfgEquipBoxTalbe;
    CfgActivityTaskTable m_cfgActivityTaskTable;
    CfgOutLinkFestivalTable m_cfgOutLinkFestivalTable;
    std::map<int, CfgYYGameApp> m_CfgYYGameAppMap;
    std::map<int, CfgYYVip> m_CfgYYVipMap;
    std::list<CfgYYSuperBuff> m_CfgYYSuperBuffList;
    std::list<CfgYYSuperBuff> m_Cfg37wanSuperBuffList;
    std::map<int, CfgSgGameApp> m_CfgSgGameAppMap;
    CfgEquipTable m_cfgEquip;
    CfgItemGemTable m_cfgItemGem;
    CfgItemCombiTable m_cfgItemCombi;
    CfgPetTable m_cfgPetTable;
    CfgSkillLevelUpTable m_cfgSkillLevelUpTable;
    CfgTaskCycleTable m_cfgTaskCycleTable;
    CfgFamilyTable m_cfgFamilyTable;
    CfgFamilyWarTable m_cfgFamilyWarTable;
    CfgFamilyLightExpTable m_cfgFamilyLightExpTable;
    CfgHorseRacingRewardTable m_cfgHorseRacingRewardTable;
    CfgCampWarTable m_cfgCampWarTable;
    CfgCityWarTable m_cfgCityWarTable;
    CfgHallOfFameTable m_cfgHallOfFameTable;
    CfgTitleTable m_cfgTitleTable;
    std::map<int, CfgDropRecord> m_cfgDropRecordTable;
    CfgDungeonScoreTable m_cfgDungeonScoreTable;
    std::map<std::pair<int, int>, CfgDungeonSummon> m_cfgDungeonSummon;
    CfgGoldEggTable m_cfgGoldEggTable;
    CfgLimitTimeTable m_cfgLimitTimeTable;
    CfgMysteryShopTable m_cfgMysteryShopTable;
    std::map<int, CfgBuyGift> m_cfgBuyGiftTable;
    CfgExchangeTable m_cfgExchangeTable;
    CfgMysteryGiftTable m_cfgMysteryGiftTable;
    CfgDrawTable m_cfgDrawTable;
    CfgMapRoadTable m_cfgMapRoadTable;
    CfgTrailerTable m_cfgTrailerTable;
    CfgMYSJRewardTable m_cfgMYSJRewardTable;
    CfgMaintainCompensateTable m_cfgMaintainCompensateTable;
    CfgWishRewardTable m_cfgWishRewardTable;
    CfgBFZLEnterCostTable m_cfgBFZLEnterCostTable;
    CfgBlacketMarketTable m_cfgBlacketMarketTable;
    std::map<int, int> m_cfgLevelChatTable;
    std::map<std::string, CfgSuperMember> m_cfgSuperMember;
    std::map<std::string, CfgSouGouSkin> m_cfgSouGouSkin;
    CfgPetEquipTable m_cfgPetEquipTable;
    CfgWeiXinTable m_cfgWeiXinTable;
    CfgAdultGiftTable m_cfgAdultGiftTable;
    CfgTencentTable m_cfgTencentTable;
    CfgFamilyDungeonTable m_cfgFamilyDungeonTable;
    cfgGoblinTableData m_cfgGoblinTableData;
    cfgSpecialMonsterTable m_cfgSpecialMonsterTable;
    cfgEquipUpPhaseTable m_cfgEquipUpPhaseTable;
    CfgTouZiTable m_cfgTouZiTable;
    CfgSearchBackTable m_cfgSearchBackTable;
    CfgEquipBlessTable m_cfgEquipBlessTable;
    std::list<PkDropRate> m_PkDropRateList;
    std::map<int, CfgSysMail> m_sysMail;
    std::map<int, BossInfo> m_BossInfo;
    FaBaoTable m_FaBaoTable;
    std::map<int, std::vector<CfgQuestions>> m_mQuestions;
    std::map<int, CfgGameShop> m_GameShopMap;
    CQiFuTable m_QiFuTable;
    VipTable m_VipTable;
    std::map<signed char, CfgSignReward> m_signReward;
    std::map<signed char, CfgOnlineReward> m_OnLineReward;
    std::map<signed char, CfgSevenLoginRewrad> m_SevenLoginReward;
    std::map<int, CfgLevelGift> m_LevelGift;
    std::map<int, CfgWeekOnlineReward> m_WeekOnlineReward;
    std::map<int, CfgOffLineExp> m_OffLineExpTable;
    std::map<int, VipCardCfg> m_VipCardTable;
    std::map<int, FunctionOpenCfg> m_FunctionOpenCfg;
    std::map<int, std::vector<CfgItemGift>*> m_PetGifts;
    std::map<int, CfgPlantEventEffect> m_PlantEventMap;
    std::map<signed char, CfgEquipExchange> m_CfgEquipExchangeMap;
    CfgShouChongTable m_cfgShouChongTable;
    std::map<signed char, NewServerFavorable> m_NewServerFavorable;
    NewServerFavorable m_ThreePetGift;
    std::list<CfgEverydayChongZhi> m_EveryDayChongZhiTable;
    std::map<int, CfgKaiFuHuoDongData> m_KaiFuHuoDongTable;
    std::map<int, CfgHuoYueDu> m_HuoYueDuTable;
    std::map<signed char, CfgHuoYueDuReward> m_HuoYueDuRewardTable;
    std::list<CfgAppendAttr> m_AppendAttrTable;
    std::list<SpecialTreasureMapRandCfg> m_SpecialTreasureMapRandCfgList;
    CfgFamilyTask m_FamilyTaskTable;
    std::map<int, FamilyTaskReward> m_FamilyTaskReward;
    std::map<signed char, CfgTotalChongZhi> m_TotalChongZhiTable;
    std::map<int, std::list<CfgMonsterAddAttr>> m_MonstAddAttrMap;
    std::map<int, DaZheQuan> m_DaZheQuanMap;
    std::map<int, Wan360Reward> m_Wan360RewardMap;
    std::map<std::string, int> m_Wan360RewardTypeMap;
    std::set<std::string> m_ItemLog;
    CfgVplan m_CfgVplan;
    Speed360Reward m_Speed360Reward;
    std::map<int, std::vector<GroupMonster>> m_GroupMonsterMap;
    std::map<int, BuyFaBaoResCfg> m_FaBaoResMap;
    std::map<int, XingMaiCfg> m_XingMaiCfgTable;
    std::map<int, CfgXingMaiSlot> m_cfgXingMaiSlotTable;
    std::map<int, TianLingCfg> m_TianLingCfgTable;
    std::map<int, DamnationCfg> m_DamnationCfgTable;
    std::map<int, ScoreShopCfg> m_ScoreShopCfgTable;
    ChouJiangTable m_ChouJiangTable;
    CLibraryTable m_CLibraryTable;
    TreasureMapTabale m_TreasureMapTabale;
    std::map<int, BackEquipTask> m_EquipBackTable;
    std::map<int, std::map<int, std::list<Param2>>> m_EquipBackTaskRate;
    std::map<int, SpeciaEquipCfg> m_SpeciaEquipCfgMap;
    std::map<std::pair<int, int>, SpeciaSkillDistance> m_SpeciaSkillDistanceTable;
    LuckDropTable m_LuckDropTable;
    std::map<int, LevelRefinCfg> m_LevelRefinCfgMap;
    std::map<int, CfgBossFirstKilled> m_CfgBossFirstKilledMap;
    std::map<int, UltimateChallengeCfg> m_UltimateChallengeCfgMap;
    std::map<int, CfgCachet> m_CfgCachetMap;
    std::map<signed char, BossKilledReward> m_BossKilledRewardMap;
    std::map<int, SystemOpenGift> m_SystemOpenGiftMap;
    std::map<int, SpecialMap> m_SpecialMapMap;
    std::map<int, TestServerReward> m_TestServerRewardMap;
    std::map<int, GuWuCfg> m_GuWuCfgMap;
    std::map<int, FunctionOpenMail> m_FunctionOpenMailMap;
    std::list<XiangYaoTaskCfg> m_XiangYaoTaskCfgList;
    std::map<int, ShenWeiTaskCfg> m_ShenWeiTaskCfgMap;
    std::map<std::string, CfgMobilePhoneGift> m_CfgMobilePhoneGift;
    std::map<std::string, CfgMiniClient> m_CfgMiniClient;
    std::map<int, std::list<CfgWuHunShop>> m_CfgWuHunShopMap;
    std::map<int, CfgWuHunShop> m_CfgWuHunShopItemMap;
    std::map<int, WuHunItem> m_WuHunItemMap;
    std::map<int, CreateWuHun> m_CreateWuHunMap;
    BossDistribution m_BossDistribution;
    std::map<int, SpecialBossMapCfg> m_SpecialBossMapCfgMap;
    std::map<int, DuiHuanLimit> m_DuiHuanLimit;
    std::map<std::pair<int, int>, JewelPavilionCfg> m_JewelPavilionCfgMap;
    std::map<int, SuperTeHuiCfg> m_SuperTeHuiCfgMap;
    std::map<std::pair<int, int>, GoblinCfg> m_GoblinCfgMap;
    std::map<int, std::list<AddAttribute>> m_GoblinSuitMap;
    std::map<std::pair<int, int>, ShouHuRefinishingCfg> m_ShouHuRefinishingCfgMap;
    std::map<int, std::list<AddAttribute>> m_ShouHuRefinishingSuitMap;
    std::map<std::pair<int, int>, WingEquipPolish> m_WingEquipPolishCfgMap;
    std::map<int, std::list<AddAttribute>> m_WingEquipPolishSuitMap;
    std::map<std::pair<int, int>, WinRefiningCfg> m_WinRefiningCfgMap;
    std::map<int, std::list<AddAttribute>> m_WingEquipRefiningSuitMap;
    std::map<int, GuiGuDaoRenCfg> m_GuiGuDaoRenCfgMap;
    CfgShiZhuangTable m_cfgShiZhuangTable;
    MonthlyChouJiangTable m_MonthlyChouJiangTable;
    std::map<int, std::list<ActDropItem>> m_ActDropItemListMap;
    std::map<int, XunLeiCfg> m_XunLeiCfgMap;
    CfgGuardPrivilegeTable m_cfgGuardPrivilegeTable;
    CycleTowerTable m_CycleTowerTable;
    std::map<std::pair<std::string, int>, PlatformDaTing> m_PlatformDaTingMap;
    std::map<std::pair<std::string, int>, MiniClientCfg> m_GetMiniClientMap;
    std::map<std::pair<std::string, int>, PlatformVip> m_PlatformVipMap;
    std::map<int, ShangRenCfg> m_ShangRenCfgMap;
    std::map<int, SelectItemCfg> m_SelectItemCfgMap;
    std::map<std::pair<std::string, int>, PlatformRewardCfg> m_PlatformRewardCfgMap;
    std::map<int, DiligenceCfg> m_DiligenceCfgMap;
    std::map<int, ContributionCfg> m_ContributionCfgMap;
    std::map<int, HoeCfg> m_HoeCfgMap;
    CMingGeTable m_CMingGeTable;
    std::map<int, CrossTowerCfg> m_CrossTowerCfgMap;
    KunLingTable m_KunLingTable;
    std::map<int, LuDaShiVip> m_LuDaShiVipMap;
    std::map<int, FlopDrawCfg> m_FlopDrawCfgMap;
    SevenTaskTable m_SevenTaskTable;
    std::map<int, MoFuSkillCfg> m_MoFuSkillCfgMap;
    std::map<std::pair<int, int>, ZhuNengCfg> m_ZhuNengCfgMap;
    std::map<int, MoFuDuiHuan> m_MoFuDuiHuanMap;
    std::map<int, ChristmasDuiHuan> m_ChristmasDuiHuanMap;
    std::map<int, BaoKuFuBen> m_BaoKuFuBenMap;
    std::map<int, BaoKuRandom> m_BaoKuRandomMap;
    std::map<int, ChargeDungeonCfg> m_ChargeDungeonCfgMap;
    std::map<int, GongMingCfg> m_GongMingCfgMap;
    CfgBeastShrineTable m_CfgBeastShrineTable;
    std::map<int, int> m_TongTianChiRankReward;
    XinMoTable m_XinMoTable;
    std::map<int, std::list<RefreshMonsterCfg>> m_RefreshMonsterCfgListMap;
    EnergyCfg m_EnergyCfg;
    std::map<std::pair<int, int>, VipEquipPosLevelUp> m_VipEquipPosLevelUpMap;
    std::list<VipEQuipPosSuit> m_VipEQuipPosSuitList;
    std::map<std::pair<int, int>, ShiZhuLevelUp> m_ShiZhuLevelUpMap;
    std::list<ClbAimCfg> m_ClbAimCfgList;
    std::map<int, CVipClubLuckyDrop> m_CVipClubLuckyDropMap;
    std::map<int, ZhanHunCfg> m_ZhanHunCfgMap;
    std::map<int, LittleHelperCfg> m_LittleHelperCfgMap;
    std::map<int, ShiQuCfg> m_ShiQuCfgMap;
    ZeroBuyPetCfg m_ZeroBuyPetCfg;
    std::map<int, SunAndMoonCfg> m_SunAndMoonCfgMap;
    std::map<int, TreasureHunterCfg> m_TreasureHunterCfgMap;
    std::map<std::pair<int, int>, NationalDayTaskCfg> m_NationalDayTaskCfgMap;
    std::map<int, NationalDayLevelCfg> m_NationalDayLevelCfgMap;
    std::map<int, RongHeCfg> m_RongHeCfgMap;
    std::map<int, EquipRongHe> m_EquipRongHeMap;
    CfgFestivalActivityTable m_festivalActivityTable;
};

#endif // CFG_DATA_H