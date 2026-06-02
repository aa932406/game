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
#include "Other/RwLock.h"

namespace Answer { class MySqlDBGuard; }



struct IDataStruct {};

class PlayerDBData : public IDataStruct {
public:
    PlayerDBData();
    ~PlayerDBData();
    void CleanUp();
    void SaveToSqlString(class SqlStringList *const sqls, char (*const szSQL)[4096], int64_t nCid);
    bool LoadFromDB(class Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, int64_t nCid);
    void PackageData(class Answer::NetPacket *packet);

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

    // 宠物数据 (供 CExtCharPet 使用)
    struct {
        struct {
            int32_t nStar;
            int32_t nHuanHua;
        } petData;
    } m_CharPets;

    // 传送门数据 (供 CExtCharPortal 使用)
    struct {
        PortalInfoList lstPortal;
        int32_t nPortalId;
    } m_PortalDBData;

    // 技能数据 (供 CExtCharSkill 使用)
    struct {
        std::string talents;
        int32_t m_nPower;
    } skillData;

    // 翅膀数据 (供 CExtCharWing 使用)
    struct {
        int32_t m_Level;
        int32_t m_Luck;
        int32_t m_HuanHua;
    } m_CharWingDBData;

    // 许愿数据 (供 CExtCharWish 使用)
    struct {
        std::list<CharWishInfo> lstWishs;
    } m_WishDBData;

    // 祈福数据 (供 CExtCharWorship 使用)
    struct {
        int32_t nTimes;
        std::string strCharList;
    } m_WorshipData;

    // 周期任务数据 (供 CExtChrTaskCycle 使用)
    struct {
        int32_t nFinishTimes;
        int32_t nTaskId;
        int8_t nState;
    } taskCycleData;

    // 货币数据 (供 CExtCurrency 使用)
    struct {
        int64_t vCurrency[12];
    } m_CurrencyData;

    // 装备数据 (供 CExtEquip 使用)
    struct {
        MemChrBag vEquip[93];
    } equipData;

    // 宝石/强化/神耀数据 (供 CExtEquip 使用)
    struct {
        std::string gemInfo;
        std::string posLevel;
        std::string Strengthen;
        std::string ShenYaoEquipPos;
    } gemData;

    // 角色摘要数据 (供 CExtEquip 使用)
    struct {
        int32_t weapon;
        int32_t cloth;
        int32_t equipStar;
        int32_t equipGem;
    } chr;

    // 神秘商店数据 (供 CExtCharMysteryShop 使用)
    struct {
        std::list<MysteryShop> lstShop;
    } m_MysteryShopDBData;

    // 家族数据 (供 CExtCharFamily 使用)
    struct FamilyData {
        int32_t nFamilyId;
        int32_t nPosition;
        int32_t nContribution;
        int32_t nDailyContribution;
        std::string strSkills;
        int32_t nMedLevel;
        int32_t nMedRes;
        int32_t nHoe;
        int32_t EnterDungeionFamilyId;
        int32_t EnterCount;
        int32_t EnterTime;
    } m_FamilyData;

    // 交换数据 (供 CExtCharExchange 使用)
    struct {
        std::list<ExchangeRecord> lstExchange;
    } m_ExchangeDBData;

private:
    // TODO: other member variables
};
