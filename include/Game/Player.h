#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Common/CommonTypes.h"
#include "Game/Unit.h"
#include "Character/CExtSystemBase.h"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

// 前置声明
class PlayerDBData;
class Trailer;
class CExtSystemMgr;
class CExtFightChecker;
class CExtCharCarrier;
class CExtCharBag;
class CExtEquip;
class CExtCharSkill;
class CExtOperateLimit;
class CExtCurrency;
class ChrTask;
class CObjPet;
class CShiZhuang;
class CExtCharPet;
class ItemBuff;
#include "Activity/CZongHeYunYingHD.h"
class DailyActivity;
struct ShowIcon;
class CHuoYueDu;
class CFunctionOpen;
class CVip;
class CVplan;
class ChouJiang;
class CExtChrTaskCycle;
class CExtCharFamily;
class CXinMo;

// 角色基础数据结构
struct PlayerChrData
{
    CharId_t cid;
    int32_t level;
    int64_t exp;
    int32_t job;                // 职业
    int32_t camp;               // 阵营
    int32_t gold;               // 金币
    int32_t money;              // 铜钱
    int32_t energy;             // 精力
    int32_t pk_mode;
    int32_t pk_value;
    int32_t pk_time;
    int32_t create_time;
    int32_t level_stay_time;
    int32_t pay_click_count;
};

// 玩家防沉迷/系统用户数据
struct PlayerSysUser
{
    bool    adult;              // 是否成年
    int32_t total_online_time;  // 总在线时间
    int32_t last_login_time;    // 上次登录时间
    int32_t map_enter_time;     // 进入地图时间
};

// 快捷栏动作
struct ActionSlot
{
    int32_t id;
    int32_t type;
};

// 自动战斗设置
struct AutoFightSettings
{
    std::string pick;
    std::string fight;
};

// 网络包处理函数类型 (成员函数指针)
typedef int32_t (Player::*NetPacketHandler)(Answer::NetPacket*);

// Player 继承自 Unit
class Player : public Unit
{
public:
    Player();
    virtual ~Player();

    // 基础信息
    std::string getName() const;
    std::string getFamilyName() const;
    std::string getIP() const;
    std::string GetPassport() const;
    std::string GetPlatform() const;
    std::string GetFromWay() const;
    
    CharId_t getCid() const { return m_cid; }
    int32_t getLevel() const { return m_level; }
    int32_t GetLevel() const { return m_level; }
    int32_t getJob() const { return m_job; }
    int32_t getPkValue() const { return m_chr.pk_value; }
    int64_t getExp() const { return m_chr.exp; }
    
    // 虚函数重写
    virtual EntityId_t getId() const override { return m_cid; }
    virtual bool isAlive() const override;
    int32_t GetViewHP() const;
    int32_t GetViewMaxHP() const;
    virtual int32_t GetMoveSpeed() const;
    virtual int32_t GetAttackSpeed() const;
    int32_t onUpdate(int64_t curTick);

    // 战斗相关
    void SetInFight();
    void SetInPvP();
    void checkDie();
    void postDamage(int32_t damage, UnitHandle launcher, int32_t Mid);
    void onDamageEvent(UnitHandle target, int32_t attackValue);
    void onDamagedEvent(UnitHandle launcher, int32_t attackValue);
    void onDropItem(Player* pKiller, int32_t Mid);
    
    // 状态相关
    bool IsDead() const { return m_bDie; }
    int64_t GetDieTick() const { return m_nDieTick; }
    void SetLevel(int32_t Level);
    void SetCamp(int8_t nCamp);
    void SetPkMode(int32_t mode, bool backUp);
    void setPkMode(int32_t mode, bool backUp);
    void SetPkProtectTime(int32_t Time);
    void SetActState(int8_t ActState);
    void setTrailer(Trailer* pTrailer);
    void CheckTrailer();
    void setBossRankDie();
    void SetDieTick();
    void SetStartProtect(int32_t nTime);
    DailyActivity* GetPlayerDailyActivity();
    
    // 地图/位置相关
    int32_t switchMap(Map* pMap, int32_t x, int32_t y, bool isFly);
    int32_t leaveDungeon();
    int32_t leaveActivity();
    void moveToReviveRegion(bool bInAct);
    void setOldPosition();
    void getOldPosition(PlayerPosition* retstr);
    bool isInRectangle(Position x, Position y) const;
    int32_t getActivityBirthRegion(Int32Vector* pRegsions);
    void EnterMapGongGao(int32_t GongGaoId, int32_t MapId);
    void resetPkModeOnEnterMap(const Map* pMap);
    
    // 属性重算
    void RecalcAttr() { recalcAttr(true, true); }

    // 经验/等级
    void addExp(int64_t addon, int64_t BaseExp, bool Iswallow);
    void delExp(int64_t nValue);
    void sendExpGainInfo(int64_t value, int64_t BaseExp, BenefitType benefitType);
    int64_t GetLevelExp() const;
    int64_t GetLevelMaxExp() const;
    bool upgradeLevel(bool bAuto);
    bool autoUpgradeLevel();
    void LevelUped(int32_t OldLevel, int32_t NewLevel);
    int32_t getCreatedDays() const;
    void SetLevelStartTime();
    void FullHPAndSync();
    
    // 货币相关
    int64_t GetCurrency(CURRENCY_TYPE nType) const;
    void syncGold(int32_t nGold);
    bool UseRechargeCard(int32_t nValue, bool bReal);
    void SendRechargeNotice(int32_t nGold);
    void PayedDispose(const int32_t AddGold);
    void SendRechargeGold(const int32_t Gold);
    void syncTodayGoldPay();
    void UpdateGmGold();
    
    // Buff
    void initBuff(const MemChrBuffVector* buffVt);

    // 物品相关
    void getBagSlotData(MemChrBag* retstr, int32_t slot) const;
    bool hasItem(int32_t baseid, int32_t type, int32_t count) const;
    int32_t getMainWeaponId();
    int32_t getClothesId();
    int32_t getWeaponId();
    int32_t getShiZhuangWeapon();
    int32_t getShiZhuangClothes();
    int32_t getShiZhuangFeet();
    
    // 属性计算
    void setBaseAttr();
    void recalcAttr(bool bNow, bool bInit);
    void CalBattle();
    double benefitRatio() const;
    BenefitType benefitType() const;
    void AddBeiGongAttr(int32_t nType, int32_t nValue);
    void SendBeiGongAttr();
    
    // 连接信息
    int8_t getConnId() const { return m_connid; }
    int16_t getGateIndex() const { return m_cgindex; }
    
    // 静态包装方法 (用于C风格静态调用)
    static int8_t getConnId(Player* player) { return player ? player->m_connid : -1; }
    static CharId_t getCid(Player* player) { return player ? player->m_cid : 0; }
    static int32_t getLevel(Player* player) { return player ? player->m_level : 0; }
    static int32_t getJob(Player* player) { return player ? player->m_job : 0; }
    static int16_t getGateIndex(Player* player) { return player ? player->m_cgindex : 0; }
    static int64_t getSid(Player* player) { return player ? player->m_cid : 0; }

    // 货币操作
    bool AddCurrency(CURRENCY_TYPE nType, int64_t nAmount, CURRENCY_CHANGE_REASON nReason, int32_t nParam) {
        switch (nType) {
        case CURRENCY_TYPE::CURRENCY_GOLD:
            m_chr.gold += static_cast<int32_t>(nAmount);
            break;
        case CURRENCY_TYPE::CURRENCY_MONEY:
            m_chr.money += static_cast<int32_t>(nAmount);
            break;
        case CURRENCY_TYPE::CURRENCY_CASH:
            m_cash += nAmount;
            break;
        default:
            return false;
        }
        return true;
    }
    
    bool DecCurrency(CURRENCY_TYPE nType, int64_t nAmount, CURRENCY_CHANGE_REASON nReason, int32_t nParam) {
        switch (nType) {
        case CURRENCY_TYPE::CURRENCY_GOLD:
            if (m_chr.gold < nAmount) return false;
            m_chr.gold -= static_cast<int32_t>(nAmount);
            break;
        case CURRENCY_TYPE::CURRENCY_MONEY:
            if (m_chr.money < nAmount) return false;
            m_chr.money -= static_cast<int32_t>(nAmount);
            break;
        default:
            return false;
        }
        return true;
    }

    // 网络/同步
    bool appendInfo(Answer::NetPacket* packet) const;
    void sendBasicInfo(int32_t reason);
    void sendChrInfo();
    void sendCharAttrInfo();
    void appendCharAttrInfo(Answer::NetPacket* packet);
    void sendLoginInfo();
    void sendActionList();
    void syncBasicData();
    void broadcastRevive();
    void addNetPacket(Answer::NetPacket* inPacket, uint32_t rsize);
    void checkSyncStatus();
    void sendItemEffect(const std::string* effect);
    void sendGainInfo(int32_t type, int64_t value, BenefitType benefitType);
    void sendKillerRankSelf();
    void sendPreventWallow();
    void sendChrLoginInInfo();
    void sendGambel(const MemChrBagVector* item);
    void sendPublicChat(int32_t channel, Answer::NetPacket* inPacket);
    void SendPublicChat(int32_t channel, Answer::NetPacket* inPacket);
    int32_t ChatValidateed(Answer::NetPacket* inPacket);
    void sendAttrAddon(const AttrAddonList* attrs);
    void sendAutoFight();
    void sendSystemSetting();
    
    // 数据库
    void saveToDB(int32_t reason, int32_t param, int32_t logout_time);
    void SaveDBData(PlayerDBData* dbData);
    void init(PlayerDBData* dbData);
    bool refresh();
    void reset();
    
    // 任务相关
    void setTaskCanSubmit(int32_t tid);
    void checkTaskCanSubmit(int32_t dungeonID, int8_t dungeonType);
    
    // 活动相关
    int32_t EnterActivity(int32_t nActId, int32_t nProc);
    int32_t doTeleportActivity(int32_t aid);
    int32_t doTeleport(int32_t nId);
    
    // 静态包装方法 - 信息获取
    static void GetPassport(Player* player, std::string* out) { if (player && out) *out = player->m_passport; }
    static void getName(Player* player, std::string* out) { if (player && out) *out = player->m_name; }
    static int64_t GetMoneyBindAndNoBind(Player* player) { return player ? player->m_chr.money : 0; }
    static CExtOperateLimit* GetOperateLimit(Player* player) { return player ? player->m_extOperateLimit : nullptr; }
    static ChrTask* GetTask(Player* player) { return player ? player->m_task : nullptr; }
    static CXinMo* GetCXinMo(Player* player) { return nullptr; }
    static void sendBuyItemInfo(Player* player, int32_t itemId, int8_t itemClass, int32_t count, int32_t costValue, int32_t index) {}
    static bool DecMoneyAndNoBind(Player* player, int32_t amount, CURRENCY_CHANGE_REASON reason, int32_t param) { return true; }
    static bool AddCurrency(Player* player, CURRENCY_TYPE type, int64_t amount, CURRENCY_CHANGE_REASON reason, int32_t param) { return player ? player->AddCurrency(type, amount, reason, param) : false; }
    static bool DecCurrency(Player* player, CURRENCY_TYPE type, int64_t amount, CURRENCY_CHANGE_REASON reason, int32_t param) { return player ? player->DecCurrency(type, amount, reason, param) : false; }
    
    // 图标状态
    static void SendIconState(Player* player, const ShowIcon* stu);

    // 网络包处理
    void initNetPacketHandlers();
    void setNetPacketHandler(int32_t proc, NetPacketHandler handler);
    NetPacketHandler getNetPacketHandler(int32_t proc) const;
    
    // 各种协议处理函数
    int32_t onMove(Answer::NetPacket* inPacket);
    int32_t onTrailerMove(Answer::NetPacket* inPacket);
    int32_t onJump(Answer::NetPacket* inPacket);
    int32_t onHit(Answer::NetPacket* inPacket);
    int32_t onUnitThrowed(Answer::NetPacket* inPacket);
    int32_t onKickOutMove(Answer::NetPacket* inPacket);
    int32_t onSwitchMap(Answer::NetPacket* inPacket);
    int32_t onLogout(Answer::NetPacket* inPacket);
    void onLogout(int32_t nReason, int32_t nParam);
    int32_t onSyncTime(Answer::NetPacket* inPacket);
    void SyncTime();
    int32_t onPickDropItem(Answer::NetPacket* inPacket);
    int32_t onUseTrap(Answer::NetPacket* inPacket);
    int32_t onEnterDungeon(Answer::NetPacket* inPacket);
    int32_t onLeaveDungeon(Answer::NetPacket* inPacket);
    int32_t onTalkWithNpc(Answer::NetPacket* inPacket);
    int32_t onReceiveTask(Answer::NetPacket* inPacket);
    int32_t onSubmitTask(Answer::NetPacket* inPacket);
    int32_t onGiveUpTask(Answer::NetPacket* inPacket);
    int32_t onSetTaskCanSubmit(Answer::NetPacket* inPacket);
    int32_t onSafeRevive(Answer::NetPacket* inPacket);
    int32_t onSiteRevive(Answer::NetPacket* inPacket);
    int32_t onStrongRevive(Answer::NetPacket* inPacket);
    int32_t onSwitchPkMode(Answer::NetPacket* inPacket);
    int32_t onQueryChrInfo(Answer::NetPacket* inPacket);
    int32_t onUpgradeLevel(Answer::NetPacket* inPacket);
    int32_t onQueryTaskList(Answer::NetPacket* inPacket);
    int32_t onTeleport(Answer::NetPacket* inPacket);
    int32_t onTeleportActivity(Answer::NetPacket* inPacket);
    int32_t onSetAutoFight(Answer::NetPacket* inPacket);
    int32_t onSetSystemSetting(Answer::NetPacket* inPacket);
    int32_t onBuyChrShopItem(Answer::NetPacket* inPacket);
    int32_t onBuyBackChrShopItem(Answer::NetPacket* inPacket);
    int32_t onBuyTaskCount(Answer::NetPacket* inPacket);
    int32_t onBuyResource(Answer::NetPacket* inPacket);
    int32_t onGamePublicChat(Answer::NetPacket* inPacket);
    int32_t onCrossPrivateChat(Answer::NetPacket* inPacket);
    int32_t onAddAction(Answer::NetPacket* inPacket);
    int32_t onRemoveAction(Answer::NetPacket* inPacket);
    int32_t onExchangeAction(Answer::NetPacket* inPacket);
    int32_t onQuickDone(Answer::NetPacket* inPacket);
    int32_t onBuyXuWuValue(Answer::NetPacket* inPacket);
    int32_t onEnterActivity(Answer::NetPacket* inPacket);
    int32_t onLeaveActivity(Answer::NetPacket* inPacket);
    int32_t onActivityChangeMap(Answer::NetPacket* inPacket);
    int32_t OnCheckAccelerator(Answer::NetPacket* inPacket);
    int32_t OnLevelPrison(Answer::NetPacket* inPacket);
    int32_t OnSubPkValus(Answer::NetPacket* inPacket);
    int32_t OnCycleTowerEvent(Answer::NetPacket* inPacket);
    int32_t OnRollTheDice(Answer::NetPacket* inPacket);
    int32_t OnPaiMaiHangHanHua(Answer::NetPacket* inPacket);
    int32_t OnGetMapBossInfo(Answer::NetPacket* inPacket);
    int32_t OnGetLevelBossInfo(Answer::NetPacket* inPacket);
    int32_t OnEnterSpecialBossMap(Answer::NetPacket* inPacket);
    int32_t OnLeaveSpecialBossMap(Answer::NetPacket* inPacket);
    int32_t OnChristmasDuiHuan(Answer::NetPacket* inPacket);
    int32_t onDungeonRandom(Answer::NetPacket* inPacket);
    int32_t OnBuyJingLiValue(Answer::NetPacket* inPacket);
    int32_t OnBuyRandomPosTimes(Answer::NetPacket* inPacket);
    int32_t onFamilyWarActivePillar(Answer::NetPacket* inPacket);
    int32_t GetDungeonDailyEnterLimit(int32_t nDungeonId);
    int32_t onBuyDungeonEnterTime(Answer::NetPacket* inPacket);
    int32_t onDungeonQuickDone(Answer::NetPacket* inPacket);
    int32_t onDungeonBuildTower(Answer::NetPacket* inPacket);
    int32_t onDungeonBuyTower(Answer::NetPacket* inPacket);
    int32_t onGuessTheSize(Answer::NetPacket* inPacket);
    int32_t onDungeonYJSKGuWu(Answer::NetPacket* inPacket);
    int32_t onDungeonStart(Answer::NetPacket* inPacket);
    int32_t onDungeonSelectReward(Answer::NetPacket* inPacket);
    int32_t onDungeonSummonBoss(Answer::NetPacket* inPacket);
    int32_t onDungeonSummon(Answer::NetPacket* inPacket);
    int32_t onDungeonSaoDang(Answer::NetPacket* inPacket);
    int32_t onDungeonReset(Answer::NetPacket* inPacket);
    int32_t onEnterChargeDungeon(Answer::NetPacket* inPacket);
    int32_t onUpdateFlyIconInt(Answer::NetPacket* inPacket);
    int32_t onSetGuaJi(Answer::NetPacket* inPacket);
    int32_t onQueryKillerRankSelf(Answer::NetPacket* inPacket);
    int32_t onTeleportByItem(Answer::NetPacket* inPacket);
    int32_t onClickPayButton(Answer::NetPacket* inPacket);
    int32_t onMapEntered(Answer::NetPacket* inPacket);
    int32_t onDebugCmd(Answer::NetPacket* inPacket);
    int32_t onQueryPlayerInfo(Answer::NetPacket* inPacket);
    int32_t onSocialUseWeiXinCard(Answer::NetPacket* inPacket);
    int32_t onSocialUseExchangeCode(Answer::NetPacket* inPacket);
    int32_t OnAskBossInfo(Answer::NetPacket* inPacket);
    int32_t OnRequestActivityInfo(Answer::NetPacket* inPacket);
    int32_t OnRequestActivityRankInfo(Answer::NetPacket* inPacket);
    int32_t OnActivityWorldBossGuWu(Answer::NetPacket* inPacket);
    int32_t OnActivityGetDailyReward(Answer::NetPacket* inPacket);
    int32_t OnActivityApplyCityWar(Answer::NetPacket* inPacket);
    int32_t checkPreventWallow(Answer::NetPacket* inPacket);
    int32_t BirthdayIsRight(std::string* p_cardId);
    int32_t GetDay(int year, int month);
    bool CheckString(std::string* p_card);
    bool isPreventWallow(std::string* p_card);
    
    // PvP/PK相关
    void EnterPrisonByPlayer(Player* killer);
    void EnterPrisonByMonster(int32_t Mid);
    void KillerPlayerGongGao(Player* killer);
    void SendFamilyMemberKilled(Player* Killer);
    bool isFriendSide(Unit* pUnit);
    bool checkSkillTarget(const CfgActiveSkill* pCfg, Unit* pUnit);
    void PkModeTiShi();
    void addPkValue(int32_t addon);
    bool SubPkValues(int32_t Addon);
    void updatePkValue(Player* pKiller, Map* pMap);
    void SafeToPrison();
    void sendKilledByPlayer(CharId_t cid, const std::string& name);
    void UpdateKilledByPlayer(CharId_t KillerId);
    void sendUpdateSocialPlayerInfo(PlayerInfoIndex index, int64_t value);
    void onSendSocialData(CharId_t cid);
    
    // 每日/周期检查
    void onNewDayCome();
    void minuteCheck(bool bSend);
    void checkSaveToDB();
    void checkPool();
    void saveOnlineTime();
    void refreshDailyCheck(bool first);
    void refeshKillerRecord();
    void kickBackFromCross(int32_t Reason);
    void addLogoutPacket(int32_t reason, int32_t param);
    void LoginGongGao();
    void GetOtherQueryInfo();
    void SendOtherPlayerInfo(int8_t connid, int16_t cgindex);
    
    // 团队状态
    PlayerTeamStatus GetTeamStatus() const;
    
    // 扩展系统初始化
    void InitExtSystems();
    
    // 随机传送
    int32_t OnRandPos();
    
    // 内部网络包检查
    bool checkNetPackets();
    bool isDeadProc(ProcId_t nProc);
    bool queryBagInfo(Answer::NetPacket* inPacket, Int32Vector* vSlot);
    
    // 循环塔相关
    void CheckEnterCycleTower();
    void CheckLeaveCycleTower();
    void UpdateCycleTowerState(CYCLE_TOWER_ENVET_TYPE nType);
    void SendCycleTowerInfo();
    void SendCycleTowerTime();
    
    // 荒原相关
    void CheckLevelWastelands();
    void CheckAddWastelandsValue();
    int32_t GetMaxValue();
    void AddWastelandsValue(int32_t Value);
    
    // 灵兽殿相关
    void AddBeastShrineValue(int32_t Value);
    void CheckAddBeastShrineValue();
    void CheckBeastShrinePoints();
    void CheckBeastShrineAutoKick();
    
    // 植物相关
    void SetPlantState(bool bState);
    int32_t getAction();
    EntityId_t GetPlantId();
    
    // 节日相关
    void checkFestivalVersion();
    void ResetFestivalData(int32_t nVersion);
    void SendJingLiValue();
    void ResetJingLi();
    int32_t GetChatLevel();
    void LogProc(int32_t procId);
    void LogNpc(int32_t NpcId, int32_t Opway, int32_t OtherNpcId);
    
    // 记录相关
    void updateRecord(int32_t id, int32_t param);
    int32_t getRecord(int32_t id);
    void clearRecordRange(int32_t start, int32_t end, int32_t diffDay);
    
    // 其他
    void SetTeJieCRI();
    Position GetSummonBossPos();
    void AddDaTiExp(int32_t Index, int8_t Result);
    void TiShiInfo(int32_t TiShiId, int32_t Pos);
    bool IsActDropItemMap();
    bool checkIsInCross();
    void setNeedSyncSelf();
    void SetNeedSyncAround();
    
    // 扩展系统访问
    CExtCharBag* GetCharBag() { return m_extCharBag; }
    CExtEquip* GetEquip() { return m_extEquip; }
    CExtOperateLimit* GetOperateLimit() { return m_extOperateLimit; }
    CExtCharSkill* GetCharSkill() { return m_extCharSkill; }
    CExtFightChecker* GetFightChecker() { return m_extFightChecker; }
    CExtCharCarrier* GetCharCarrier() { return m_extCharCarrier; }
    CExtCurrency* GetCurrency() { return m_extCurrency; }
    PlayerChrData* GetChrData() { return &m_chr; }
    
    // 活动相关子系统
    CHuoYueDu* GetPlayerHuoYueDu();
    CFunctionOpen* GetPlayerFunctionOpen();
    CVip* GetPlayerVip();
    CVplan* GetVplan();
    ChouJiang* GetPlayerChouJiang();
    CExtChrTaskCycle* GetCharTaskCycle();
    CExtCharFamily* GetCharFamily();

protected:
    // 基本成员变量
    PlayerChrData m_chr;            // 角色基础数据
    
    CharId_t    m_cid;              // 角色ID
    int32_t     m_level;            // 等级
    int32_t     m_job;              // 职业
    int32_t     m_Battle;           // 战力
    int64_t     m_exp;              // 经验值
    int8_t      m_camp;             // 阵营
    int8_t      m_pkMode;           // PK模式
    int32_t     m_pkMode_bk;        // PK模式备份
    int8_t      m_actState;         // 活动状态
    int8_t      m_nGuaJi;           // 挂机状态
    
    std::string m_name;             // 角色名
    std::string m_familyName;       // 家族名
    std::string m_ip;               // IP地址
    std::string m_passport;         // Passport
    std::string m_platform;         // 平台
    std::string m_fromWay;          // 来源
    
    // 货币（直接成员）
    int64_t     m_money;            // 铜钱
    int64_t     m_gold;             // 金币
    int64_t     m_cash;             // 元宝/钻石
    int64_t     m_bindGold;         // 绑定金币
    int64_t     m_honor;            // 荣誉
    int64_t     m_contribution;     // 贡献
    
    // 时间相关
    int64_t     m_lastSaveTick;     // 上次保存时间
    int64_t     m_lastPoolTick;     // 上次池检查时间
    int64_t     m_levelStartTime;   // 等级开始时间
    int64_t     m_nDieTick;         // 死亡时间
    int64_t     m_startGatherTick;  // 开始采集时间
    int64_t     m_lastPickTick;     // 上次拾取时间
    int64_t     m_AcceleratorTime;  // 加速检测时间
    int64_t     m_saveDataTimeCount; // 自动保存间隔
    int64_t     m_TeJieCRI;         // 特戒CRI时间
    int64_t     m_lastPkValueTick;  // 上次PK值变化时间
    
    // 状态标记
    bool        m_needSyncStatus;   // 需要同步状态
    bool        m_needSyncAttr;     // 需要同步属性
    bool        m_needRecalAttr;    // 需要重新计算属性
    bool        m_isBackCity;       // 是否在回城
    bool        m_bDie;             // 是否死亡
    bool        m_inFight;          // 是否在战斗
    bool        m_inPvP;            // 是否在PVP
    
    int8_t      m_connid;           // 连接ID
    int16_t     m_cgindex;          // CG索引
    
    // 扩展系统 (指针，由InitExtSystems创建)
    CExtFightChecker* m_extFightChecker;
    CExtCharCarrier*  m_extCharCarrier;
    CExtCharBag*      m_extCharBag;
    CExtEquip*        m_extEquip;
    CExtCharSkill*    m_extCharSkill;
    CExtOperateLimit* m_extOperateLimit;
    CExtCurrency*     m_extCurrency;
    
    // 系统管理器 (指针，由InitExtSystems创建)
    CExtSystemMgr*      m_extSysMgr;
    
    // 其他系统引用
    ChrTask*            m_task;             // 任务系统
    CObjPet*            m_pet;              // 宠物
    Trailer*            m_trailer;          // 拖车
    
    // 网络包处理器映射 (使用map而非固定数组)
    std::map<int32_t, NetPacketHandler> m_packetHandlers;
    
    // 网络包队列
    std::list<Answer::NetPacket*> m_netPackets;
    
    // 旧位置（用于回城/活动）
    PlayerPosition m_oldPosition;
    
    // Boss排名相关
    std::list<EntityId_t> m_lBossRank;
    
    // 植物相关
    EntityId_t  m_plantId;          // 植物ID
    int32_t     m_plantState;       // 植物状态
    
    // 快捷栏动作 (10个槽位)
    ActionSlot m_actions[10];
    
    // 自动战斗设置
    AutoFightSettings m_autoFight;
    
    // 系统设置
    std::map<int32_t, int32_t> m_systemSettings;
    
    // 防沉迷数据
    PlayerSysUser m_sysUser;
    
    // 背攻属性
    std::map<int32_t, int32_t> m_BeiGongAttr;
    
    // 综合运营活动
    CZongHeYunYingHD   m_PlayerYunYingHD;
    
    // 处理过的协议日志列表
    std::set<int32_t> m_setProcedLogs;

    // Buff管理器 (继承自Unit, 在Unit.h中定义)
    
private:
    void safeRevive();
};

#endif // _PLAYER_H_
