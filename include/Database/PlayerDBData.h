#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Common/CommonTypes.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtCharExchange.h"
#include "Character/CExtCharMysteryShop.h"
#include "Character/CExtCharPortal.h"
#include "Character/CExtCharWish.h"
#include "Character/CExtOperateLimit.h"
#include "Character/CharLittlerHelper.h"

#include "Activity/DailyActivityData.h"

#include "Database/CurrencyDBData.h"
#include "Database/OperateLimitDBData.h"
#include "Database/PetDBData.h"
#include "Database/MailDBData.h"
#include "Database/CharFamilyDBData.h"
#include "Database/WorshipDBData.h"
#include "Database/MysteryShopDBData.h"
#include "Database/ExchangeDBData.h"
#include "Database/CharWishDBData.h"
#include "Database/CharWingDBData.h"
#include "Database/MagicBoxDBData.h"
#include "Database/WuHunShopDBData.h"
#include "Database/PortalDBData.h"
#include "Database/XinMoDBData.h"

#include "Other/MemCharacterData.h"
#include "Other/SysUserData.h"
#include "Other/MemChrEquipData.h"
#include "Other/MemChrGemData.h"
#include "Other/MemChrBagData.h"
#include "Other/MemChrSkillData.h"
#include "Other/MemChrTaskData.h"
#include "Other/MemChrTaskCycleData.h"
#include "Other/MemChrActionData.h"
#include "Other/MemChrAutoFightData.h"
#include "Other/MemChrSystemSettingData.h"
#include "Other/MemChrBuffData.h"
#include "Other/MemChrDepotData.h"
#include "Other/SysUserPreventWallowData.h"
#include "Other/MemAttrData.h"
#include "Other/FriendExpReward.h"
#include "Other/ShangChengData.h"
#include "Other/BossKilledRewardData.h"
#include "Other/CFaBaoData.h"
#include "Other/CGoblinData.h"
#include "Other/CJueWeiData.h"
#include "Other/ScoreShopData.h"
#include "Other/TouZiData.h"
#include "Other/CHuoYueDuData.h"
#include "Other/CVplanData.h"
#include "Other/VipData.h"
#include "Other/ChouJiangData.h"
#include "Other/MoneyRewardTaskData.h"
#include "Other/CShiZhuangData.h"
#include "Other/CMonthlyChouJiangData.h"
#include "Other/CMingGeData.h"
#include "Other/CKunData.h"
#include "Other/CFlopDraw.h"
#include "Other/CSevenDayData.h"
#include "Other/CLittleHelperData.h"
#include "Other/NationalDayData.h"
#include "Other/EquipRongHeData.h"
#include "Other/RwLock.h"

namespace Answer { class MySqlDBGuard; }

class PlayerDBData : public IDataStruct {
public:
    PlayerDBData();
    ~PlayerDBData();
    void CleanUp();
    void SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], int64_t nCid);
    bool LoadFromDB(class Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, int64_t nCid);
    void PackageData(class Answer::NetPacket *packet);
    static void UnPackageData(PlayerDBData* data, class Answer::NetPacket* packet, int32_t flag) { if (data) data->UnPackageData(packet, flag); }
    void UnPackageData(class Answer::NetPacket* packet, int32_t flag);

    // 背包数据 (供 CExtCharBag 使用)
    struct BagData {
        MemChrBag bagData[56];
        std::map<int32_t, int32_t> m_ItemLimit;
    } bagData;

    // 仓库数据 (供 CExtCharDepot 使用)
    struct GambleDepot {
        int32_t OpneCount;
        std::string Password;
        std::string SecondPassword;
        MemChrBag gambleDepot[560];
        int64_t Currency[12];
    } gambleDepot;

    int32_t m_nJueWei;

    // 宠物数据
    PetDBData m_CharPets;

    // 传送门数据
    PortalDBData m_PortalDBData;

    // 翅膀数据
    CharWingDBData m_CharWingDBData;

    // 许愿数据
    CharWishDBData m_WishDBData;

    // 祈福数据
    WorshipDBData m_WorshipData;

    // 翻牌数据
    CFlopDraw m_CFlopDraw;

    // 小助手数据
    CLittleHelperData m_CLittleHelper;

    // 操作限制数据
    OperateLimitDBData m_OperateLimit;

    // 货币数据
    CurrencyDBData m_CurrencyData;

    // 神秘商店数据
    MysteryShopDBData m_MysteryShopDBData;

    // 家族数据
    CharFamilyDBData m_FamilyData;

    // 交换数据
    ExchangeDBData m_ExchangeDBData;

    // 角色摘要数据 (匿名struct，供外部通过 chr.data.cid 等访问)
    struct {
        int32_t weapon;
        int32_t cloth;
        int32_t equipStar;
        int32_t equipGem;
        CharacterData data;
    } chr;

    // 装备数据 (匿名struct，供外部通过 equipData.vEquip 等访问)
    struct {
        EquipSlot vEquip[93];
    } equipData;

    // 宝石/强化/神耀数据 (匿名struct，供外部通过 gemData.gemInfo 等访问)
    struct {
        std::string gemInfo;
        std::string posLevel;
        std::string Strengthen;
        std::string ShenYaoEquipPos;
    } gemData;

    // 技能数据 (匿名struct，供外部通过 skillData.talents 等访问)
    struct {
        std::string talents;
        int32_t m_nPower;
    } skillData;

    // 周期任务数据 (匿名struct，供外部通过 taskCycleData.nTaskId 等访问)
    struct {
        int32_t nFinishTimes;
        int32_t nTaskId;
        int8_t nState;
    } taskCycleData;

    // ========== 额外添加的缺失成员 ==========

    // 基本状态
    int8_t connid;
    int16_t cgindex;
    int32_t loadTime;
    int32_t saveRefCount;

    // 类型化数据成员 (DB持久化用)
    SysUserData sysUser;
    DailyActivityData signInfo;
    MemChrTaskData taskData;
    MemChrActionData actionData;
    MemChrAutoFightData autoFight;
    MemChrSystemSettingData systemSetting;
    MemChrBuffData buffData;
    SysUserPreventWallowData sysUserPreventWallow;
    MemAttrData attrData;
    MemChrDepotData depotData;
    FriendExpReward m_FriendExpReward;
    MailDBData m_MailDBData;
    ShangChengData m_ShangChengData;
    BossKilledRewardData m_BossKilledReward;
    CFaBaoData m_FaBaoData;
    CGoblinData m_CGoblinData;
    CJueWeiData m_JueWeiData;
    ScoreShopData m_ScoreShopData;
    TouZiData m_TouZiData;
    CHuoYueDuData m_HuoYueDuData;
    CVplanData m_VplanData;
    MagicBoxDBData m_MagicBoxDBData;
    VipData m_vipData;
    ChouJiangData m_ChouJinagData;
    MoneyRewardTaskData m_MoneyRewardTaskData;
    WuHunShopDBData m_WuHunShopDBData;
    CShiZhuangData m_ShiZhuangData;
    CMonthlyChouJiangData m_CMonthlyChouJiangData;
    CMingGeData m_CMingGeData;
    CKunData m_CKunData;
    CSevenDayData m_CSevenDayData;
    XinMoDBData m_XinMoDBData;
    NationalDayData m_NationalDayData;
    EquipRongHeData m_EquipRongHeData;
};
