#ifndef _COMMONTYPES_H_
#define _COMMONTYPES_H_

#include "Utility/Position.h"
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <mutex>

// ============================================================
// 基础类型定义 (从反编译代码还原)
// ============================================================

typedef int64_t  CharId_t;       // 角色ID
typedef int64_t  EntityId_t;     // 实体ID
typedef uint16_t ProcId_t;       // 协议处理ID
typedef int32_t  SkillId_t;      // 技能ID
typedef int64_t  FamilyId_t;     // 家族ID
typedef int32_t  Job_t;          // 职业
typedef int32_t  socket_t;       // Socket 文件描述符
typedef int32_t  MapId_t;        // 地图ID

// ============================================================
// 前置声明（用于类型别名）
// ============================================================

struct Position;
struct ItemData;
class Player;
class Unit;
class StaticObj;

// ============================================================
// 常用容器类型别名
// ============================================================

typedef std::vector<int32_t>   Int32Vector;
typedef std::list<int32_t>     Int32List;
typedef std::list<int8_t>      Int8List;
typedef std::vector<Position>  PositionVector;
typedef std::list<Position>    PosList;
typedef std::list<Player*>     PlayerList;
typedef std::list<Unit*>       UnitList;
typedef std::list<StaticObj*>  ObjList;
typedef std::list<CharId_t>    CharIdList;

// 单位/对象容器 (用于 Map 相关方法)
typedef std::vector<Unit*>     UnitVector;

// 地图位置结构体 (用于活动NPC/植物位置)
struct MapPos
{
    int32_t x;
    int32_t y;
};
typedef std::vector<MapPos>    MapPosVector;

// ============================================================
// 枚举定义 (从反编译代码中的枚举引用还原)
// ============================================================

// 怪物状态
enum class MonsterState
{
    MS_STAND       = 0,   // 站立
    MS_WALK_AROUND = 1,   // 闲逛
    MS_RUN_AROUND  = 2,   // 逃跑
    MS_WALK_ATTACK = 3,   // 走向攻击
    MS_ATTACK      = 4,   // 攻击
    MS_GHOST       = 5,   // 幽灵态
    MS_CORPSE      = 6,   // 死亡尸体
    MS_RUN_ON_ROAD = 7,   // 沿路跑
    MS_ESCAPE      = 8,   // 逃跑
    MS_WALK_HOME   = 9    // 回家
};

enum class CURRENCY_TYPE
{
    CURRENCY_MONEY       = 0,   // 铜钱
    CURRENCY_GOLD        = 1,   // 金币
    CURRENCY_HONOR       = 2,   // 荣誉
    CURRENCY_BIND_GOLD   = 3,   // 绑定金币
    CURRENCY_CONTRIBUTION = 4,  // 贡献
    CURRENCY_XUWU        = 5,   // 虚无值
    CURRENCY_JINGLI      = 6,   // 精力值
    CURRENCY_CASH        = 7,   // 现金/绑定元宝
    CURRENCY_TYPE_COUNT  = 8    // 类型总数
};

constexpr CURRENCY_TYPE CURRENCY_INVALID = static_cast<CURRENCY_TYPE>(-1);

// 货币变化原因
#ifndef CURRENCY_CHANGE_REASON_DEFINED
#define CURRENCY_CHANGE_REASON_DEFINED
enum class CURRENCY_CHANGE_REASON
{
    CCR_NONE = 0,
    GCR_BUILD_TOWER,                  // 建造塔
    GCR_BUY_RANDOM_DUNGEON_TIMES,     // 购买随机副本次数
    GCR_DUNGEON_DOUBLE_REWARD,        // 副本双倍奖励
    GCR_DUNGEON_GUWU,                 // 副本鼓舞
    GCR_DUNGEON_REWARD,               // 副本金币奖励
    GCR_DUNGEON_SUMMON,               // 副本召唤
    GCR_XINMO_SIT_REVIVE,             // 心魔原地复活
    GCR_YJSK_GUESS_SIZE,              // 猜大小
    MCR_DUNGEON_REWARD,               // 副本铜钱奖励
    GCR_FESTIVAL_ACT_2,               // 节日活动金币消耗
    MCR_SEARCH_BACK,                  // 找回金币消耗
    GCR_GET_OFFLINE_EXP,              // 获取离线经验
    GCR_RETROACTIVE,                  // 补签
    GCC_WEEK_ONLINE_REWARD,           // 周在线奖励
    MCR_AUTO_USE,                     // 自动使用
    MCR_PICK_CURRENCY_ITEM,           // 拾取货币物品
    MCR_MAIL_CURRENCY_ITEM,           // 邮件货币物品
    MCR_CROSS_DRAW_REWARD,            // 跨服抽奖奖励
    MCR_CHR_SHOP_COST,                // 角色商店消耗
    MCR_NPC_SELL,                     // NPC出售
    GCR_BUY_LITTLE_HELPER,            // 购买小助手
};
#endif // CURRENCY_CHANGE_REASON_DEFINED

// 收益类型
enum class BenefitType
{
    BT_NORMAL     = 0,   // 普通
    BT_DOUBLE     = 1,   // 双倍
    BT_TRIPLE     = 3    // 三倍
};

// Buff效果类型
enum class BUFF_EFFECT_TYPE
{
    BET_NONE      = 0,
    BET_DAMAGE    = 1,   // 伤害
    BET_HEAL      = 2,   // 治疗
    BET_CONTROL   = 3,   // 控制
    BET_BUFF      = 4,   // 增益
    BET_DEBUFF    = 5    // 减益
};

// 瞬移原因
enum class InstanceMoveReason
{
    IMR_NORMAL    = 0,   // 正常移动
    IMR_THROWED   = 1,   // 被抛掷
    IMR_SKILL     = 2,   // 技能瞬移
    IMR_ACTIVITY  = 3,   // 活动瞬移
    IMR_TRANSFER  = 4,   // 传送
    IMR_FU_BEN    = 5,   // 副本瞬移
};

// 方向枚举 (从反编译代码还原: 使用屏幕坐标，y轴向下为正)
#ifndef DIRECTION_ENUM_DEFINED
#define DIRECTION_ENUM_DEFINED
enum class Direction
{
    DOWN        = 0,   // 下 (南/屏幕下方)
    DOWN_RIGHT  = 1,   // 右下 (东南)
    RIGHT       = 2,   // 右 (东)
    UP_RIGHT    = 3,   // 右上 (东北)
    UP          = 4,   // 上 (北/屏幕上方)
    UP_LEFT     = 5,   // 左上 (西北)
    LEFT        = 6,   // 左 (西)
    DOWN_LEFT   = 7,   // 左下 (西南)
    
    // 方向别名 (兼容写法)
    DIR_S  = 0,    // 南 = DOWN
    DIR_SE = 1,    // 东南 = DOWN_RIGHT
    DIR_E  = 2,    // 东 = RIGHT
    DIR_NE = 3,    // 东北 = UP_RIGHT
    DIR_N  = 4,    // 北 = UP
    DIR_NW = 5,    // 西北 = UP_LEFT
    DIR_W  = 6,    // 西 = LEFT
    DIR_SW = 7,    // 西南 = DOWN_LEFT
};
#endif // DIRECTION_ENUM_DEFINED

// 地图类型
enum class MapType
{
    MT_NORMAL      = 0,    // 普通地图
    MT_DUNGEON     = 1,    // 副本
    MT_ACTIVITY    = 2,    // 活动地图
    MT_PRISON      = 3,    // 监狱
    MT_TERRITORY   = 4,    // 领地
    MT_CITY_WAR    = 5,    // 城战
    MT_FAMILY_DUNGEON = 6, // 家族副本
    MT_CROSS       = 7,    // 跨服
    MT_SPECIAL_BOSS = 8,   // 特殊Boss地图
    MT_BEAST_SHRINE = 9,   // 灵兽殿
    MT_CYCLE_TOWER  = 10,  // 循环塔
    MT_WASTELANDS   = 11,  // 荒原
    MT_KUANG_DONG   = 12,  // 矿洞
    MT_XIN_MO_CUN   = 13,  // 心魔村
    MT_MO_LING_HUAN_YU = 14 // 墨灵幻域
};

// 跨服地图类型
enum class CROSS_MAP_TYPE
{
    CMT_NONE      = 0,
    CMT_CROSS     = 1
};

// 玩家队伍状态
enum class PlayerTeamStatus
{
    PTS_FREE        = 0,    // 自由
    PTS_TEAM_MEMBER = 1,    // 队员
    PTS_TEAM_LEADER = 2     // 队长
};

// 节日活动类型 (FestivalDoubleEleven用)
enum class FESTIVAL_ACTIVITY_TYPE
{
    FAT_DRAW_RANK       = 0,    // 抽奖排行
    FAT_WORLD_BOSS      = 1,    // 世界Boss
    FAT_DAILY_LI        = 2,    // 日常
    FAT_RECHARGE        = 3,    // 充值
    FAT_HUO_YUE_D       = 4,    // 活跃度
    FAT_LAND            = 5,    // 登陆
    FAT_LAND_SUM        = 6,    // 累计登陆
    FAT_DRAW            = 7,    // 抽奖
    FAT_ONLINE_T        = 8,    // 在线时间
    FAT_BEST_WISH        = 9,   // 许愿
    FAT_FA_BAO_BACK     = 10,   // 法宝返还
    FAT_EQUIP_UPSTAR    = 11,   // 装备升星
    FAT_RECHARGE_BACK   = 12,   // 充值返还
    FAT_ONLINE_TIME     = 13,   // 在线时间奖励
    FAT_DAILY_XIAO_FEI  = 14,   // 日常消费
    FAT_XIAO_FEI_SUM    = 15,   // 消费累计
    FAT_CROSS_XIAO      = 16,   // 跨服消费
    FAT_FRIEND_QI       = 17,   // 好友亲密度
    FAT_XIAO_FEI_D      = 18,   // 消费日常
    FAT_RECHARGE_D      = 19,   // 充值日常
    FAT_BOSS_SCORE      = 20,   // Boss积分
    FAT_CHOU_JIAN       = 21    // 抽奖
};

// 节日活动类型2 (CFestivalActivity用)
enum class FESTIVAL_ACTIVITY_TYPE_2
{
    FAT2_COLLECTION    = 0,    // 收集
    FAT2_DRAW          = 1,    // 抽奖
    FAT2_NPC           = 2,    // NPC活动
    FAT2_PLANT         = 3,    // 植物活动
    FAT2_DUIHUAN       = 4     // 兑换
};

// 循环塔事件类型
enum class CYCLE_TOWER_ENVET_TYPE
{
    CTET_NONE = 0
};

// 玩家信息索引
enum class PlayerInfoIndex
{
    PII_LEVEL = 0
};

// 物品变化原因
#ifndef ITEM_CHANGE_REASON_DEFINED
#define ITEM_CHANGE_REASON_DEFINED
enum class ITEM_CHANGE_REASON
{
    ICR_NONE = 0,
    IDCR_DUNGEON_RANDOM,              // 副本随机宝库
    ICR_DUNGEON_REWARD,               // 副本奖励
    ICR_DUNGEON_GUESS_SIZE,           // 副本猜大小
    ICR_WORLD_BOSS_KILL_REWARD = 700,
    IDCR_FESTIVAL_ACT_2,               // 节日活动兑换
    IDCR_DA_TI_ACT,                    // 答题活动
    ICR_SEARCH_BACK,                   // 找回
    IDCR_DOG_TITLE_GET,                // 获取狗称号
    ICR_SEVEN_LOGIN_REWARD,            // 七日登录奖励
    ICR_MAINTAIN_COMPENSATE_REWARD,    // 维护补偿奖励
    ICR_ONLINE_REWAR,                  // 在线奖励
    ICR_SIGN_REWARD,                   // 签到奖励
};
#endif

// ============================================================
// 结构体定义 (仅定义 CfgData.h 中未定义的类型)
// ============================================================

// ============================================================
// 通用结构体完整定义
// ============================================================

struct Position;
struct CfgMapRegion;

#ifndef ATTR_ADDON_DEFINED
#define ATTR_ADDON_DEFINED
struct AttrAddon {
    int index;
    int addon;
};
#endif

#ifndef ADD_ATTRIBUTE_DEFINED
#define ADD_ATTRIBUTE_DEFINED
struct AddAttribute {
    int m_nAddAttrType;
    int m_nAddAttrValue;
};
#endif

#ifndef BUFF_ATTR_DEFINED
#define BUFF_ATTR_DEFINED
struct BuffAttr {
    int attr;
    int ratio;
    int addon;
};
#endif

#ifndef ITEM_DATA_DEFINED_HERE
struct ItemData {
    int m_nId;
    int m_nClass;
    int m_nCount;
};
#define ITEM_DATA_DEFINED_HERE
#endif

#ifndef MEM_CHR_BAG_DEFINED_HERE
struct MemChrBag {
    int32_t itemId;
    int8_t itemClass;
    int32_t itemCount;
    int8_t bind;
    int32_t endTime;
    int64_t srcId;
};
#define MEM_CHR_BAG_DEFINED_HERE
#endif

// 角色背包容器
typedef std::vector<MemChrBag> MemChrBagVector;

// 掉落物品向量
typedef std::vector<ItemData> DropItemVector;

// 属性附加列表
typedef std::vector<AttrAddon> AttrAddonList;

// 玩家位置 (用于回城记录)
struct PlayerPosition
{
    int32_t x;
    int32_t y;
    int32_t mapId;
};

// 附加技能信息
struct UnitAddonSkill
{
    int32_t skillId;
    int32_t skillLevel;
};

// 活动数据结构 (用于 Monster 类)
struct ActStruct
{
    int32_t type;
    int32_t value;
    int64_t id;
};

// Buff恢复数据
struct BuffRestore
{
    int32_t attrIndex;
    int32_t value;
};

// 怪物血量事件
struct HPEvent
{
    int32_t hpPercent;
    int64_t tick;
    int32_t eventId;
};

// 怪物等待信息
struct MonsterWait
{
    int32_t id;           // 怪物ID
    int32_t delay;        // 延迟(ms)
    int32_t rewardWave;   // 奖励波次标记
    int64_t startTick;    // 开始刷新tick
    int16_t x;            // 刷新位置X
    int16_t y;            // 刷新位置Y
    int32_t broad[5];     // 广播信息(5个int = 20字节)
};

// 怪物波次
struct MonsterWave
{
    int32_t count;
    int32_t escaped;
    int32_t alive;
    int64_t startTime;
};

// Tencent信息
struct TencentInfo
{
    int32_t age;
    int8_t  fcmType;
};

// Boss击杀奖励玩家分数
struct PlayerScore
{
    CharId_t cid;
    int32_t  score;
    int32_t  damage;
};

// 角色Buff数据
struct MemChrBuff
{
    int32_t buffId;
    int8_t  index;
};

typedef std::vector<MemChrBuff> MemChrBuffVector;

// 协议ID列表
typedef std::list<ProcId_t> ProcIdList;

// 图标状态列表
struct IconState
{
    int32_t iconId;
    int8_t  state;
};

typedef std::vector<IconState> IconStateList;

// 360 TGP图标状态 (兼容旧代码)
typedef IconStateList TgpIconStateList;

// ============================================================
// 前置声明
// ============================================================

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CActivityManager;
class CfgActivity;
class CfgMonster;
class CfgNpc;
class CfgMap;
class CfgBuff;
class CfgMapMonster;
class CfgPlant;
class CfgMapPlant;
class CfgActivityMonster;
class CfgActiveSkill;
class CfgDungeon;
class Answer_NetPacket;
class Position;
class UnitHandle;
class Buff;
class StaticObj;
class Trailer;
class CObjPet;
class CObjCarrier;
class CDropItem;
class CDropItemGroup;
class Plant;
class Trap;
class CLittleHelper;
class CMapRunner;
class CMapEvent;
class Dungeon;
class IExtSystem;
class CExtSystemBase;
class CExtSystemMgr;
class CExtFightChecker;
class CExtCharBag;
class CExtCharAuction;
class CExtCharCarrier;
class CExtCharDraw;
class CExtCharExchange;
class CExtCharFamily;
class CExtCharHallOfFame;
class CExtCharJueWei;
class CExtCharMysteryGift;
class CExtCharMysteryShop;
class CExtCharPet;
class CExtCharPortal;
class CExtCharSkill;
class CExtCharTeam;
class CExtCharTeamDungeon;
class CExtCharTencent;
class CExtCharTitle;
class CExtCharWing;
class CExtCharWish;
class CExtCharWorship;
class CExtChrTaskCycle;
class CExtCurrency;
class CExtEquip;
class CExtMagicBox;
class CExtOperateLimit;
class CExtFlopDraw;
class CExtShiZhuang;
class DailyActivity;
class MonsterActivity;
class NpcActivity;
class PlantActivity;
class CFestivalActivity;
class CActivityWorldBoss;
class ChrTask;
class PlayerDBData;
class MemPetDBData;
class CTimer;
class GameService;
class DBService;
class MapManager;
class CObjAttrs;
class CObjState;
class CDBCFile;
class TencentInfo;
class MonsterWave;

namespace Answer {
    class NetPacket;
    class Random;
    class Mutex;
    template<typename T> class Singleton;
    template<typename T> class SafeQueue;
}

#endif // _COMMONTYPES_H_
