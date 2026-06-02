#ifndef _CFESTIVALDOUBLEELEVEN_H_
#define _CFESTIVALDOUBLEELEVEN_H_

#ifndef FESTIVAL_ACTIVITY_TYPE_DEFINED
#define FESTIVAL_ACTIVITY_TYPE_DEFINED

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Other/TimeArea.h"
#include "Other/MemChrBag.h"
#include "Other/FestivalRank.h"
#include "Other/ShopGoods.h"
#include "Other/RateItem.h"
#include "Other/LianRechargeCfg.h"
#include "Other/ShowIcon.h"
#include "Answer/Mutex.h"
#include "Answer/Inifile.h"

class Player;
class Map;
class Unit;
class Npc;
class CActivity;

enum FESTIVAL_ACTIVITY_TYPE : int8_t
{
    FAT_NONE = 0,
    FAT_LAND = 1,
    FAT_DAILY_RECHARGE = 2,
    FAT_DRAW = 3,
    FAT_DRAW_RANK = 4,
    FAT_EXCHANGE = 5,
    FAT_WORLD_BOSS = 6,
    FAT_HUO_YUE_DU_SUM = 7,
    FAT_DAILY_LIMIT_SHOP = 8,
    FAT_LAND_SUM = 9,
    FAT_ONLINE_TIME = 10,
    FAT_BEST_WISH = 11,
    FAT_DAILY_XIAO_FEI_RANK = 12,
    FAT_XIAO_FEI_DRAW = 13,
    FAT_MO_YU_SHI_JIE = 14,
    FAT_DAILY_RECHARGE_2 = 15,
    FAT_XIAO_FEI_SUM_RANK = 16,
    FAT_GIFT_SHOP = 17,
    FAT_FA_BAO_BACK = 19,
    FAT_RECHARGE_SUM_GIFT = 21,
    FAT_XIAO_FEI_SUM_GIFT = 22,
    FAT_BUY_PET_GIFT = 23,
    FAT_DAILY_LIMIT_SHOP_2 = 25,
    FAT_EQUIP_UPSTAR_BACK = 26,
    FAT_RECHARGE_DRAW = 27,
    FAT_BUY_GIFT = 28,
    FAT_RECHARGE_BACK = 29,
    FAT_PET_ILLUSION_ITEM = 31,
    FAT_RECHARGE_SUM_RANK = 32,
    FAT_CROSS_RECHARGE_SUM_RANK = 33,
    FAT_CROSS_XIAO_FEI_SUM_RANK = 34,
    FAT_CROSS_CHOU_JIANG_RANK = 37,
    FAT_BOSS_SCORE_RANK = 43,
    FAT_BOSS_SCORE_DRAW = 44,
    FAT_GOU_WU_CHE = 45,
    FAT_LIAN_CHONG = 46,
    FAT_CHOU_JIANG_SPECIAL = 47,
    FAT_MYSTERY_SHOP = 48,
    FAT_FRIEND_QING_YI = 49,
    FAT_HUO_YUE_DU = 50,
    FAT_MAX = 51
};
#endif // FESTIVAL_ACTIVITY_TYPE_DEFINED

class CFestivalDoubleEleven
{
public:
    CFestivalDoubleEleven();
    ~CFestivalDoubleEleven();

    void OnDaySwitch();
    void OnNewMinute(int32_t nMinute);
    void Init(int32_t line);
    void initOpenList(const std::string& str);
    void HotUpdate();
    void initCfgData();
    void loadRankData(int32_t line);
    void resetRankData(int32_t line);
    void resetPlayerData();
    int32_t loadVersion(int32_t line);
    void saveVersion(int32_t line);
    void loadDrawRank();
    void checkDrawRankInvalid(int8_t connid);
    void checkDrawRank();
    void sendDrawRankMail();
    void SendIconState(Player *player);
    void hideIcon(int32_t nIconId);
    void SendActivityInfo(Player *player);
    bool IsInTime();
    bool needShowIcon();
    bool IsInTime(FESTIVAL_ACTIVITY_TYPE nType);
    bool IsOpen(FESTIVAL_ACTIVITY_TYPE nType);
    bool CheckDropGroup(int32_t nDropGroup);
    int32_t GetLandGift(Player *player);
    int32_t GetLandSumGift(Player *player, int8_t nIndex);
    int32_t GetDrawGift(Player *player);
    int32_t GetOnlineGift(Player *player);
    int32_t GetWishGift(Player *player);
    int32_t BuyDailyLimitShopItem(Player *player);
    bool BuyTitle(Player *player);
    int32_t BuyGiftItem(Player *player, int8_t nIndex);
    int32_t GetRechargeBack(Player *player);
    int32_t GetEquipUpStarBackItem(Player *player);
    void GongGao(int32_t GongGaoId, Player *player);
    void AddDrawTimes(Player *player, int32_t nCount);
    void AddHuoYueDu(Player *player, int32_t nValue);
    int32_t getLeftTime();
    bool canGetLandGift(Player *player);
    bool canGetLandSumGift(Player *player);
    bool canGetDrawGift(Player *player);
    bool canGetOnlineGift(Player *player);
    int32_t GetOnlineTimeRewardTime(Player *player);
    bool canGetWishGift(Player *player);
    bool canGetFaBaoBack(Player *player);
    bool canGetEquipUpStarBack(Player *player);
    bool canGetRechargeBack(Player *player);
    int32_t getLandSum(Player *player);
    void SendWorldBossIconState();
    void checkWorldBoss();
    void broadcastWorldBossStart();
    void broadcastWorldBossEnd();
    void AddOnlineRecord(Player *player);
    void UpdateXiaoFeiRank(Player *player);
    void AddXiaoFeiRecord(Player *player, int32_t nCount);
    int32_t GetXiaoFeiSumGiftCount(Player *player);
    void AddFriendQingYi(Player *player, int32_t nValue);
    bool CanUseXiaoFeiDraw(Player *player);
    bool CanUseRechargeDraw(Player *player);
    void loadXiaoFeiRank();
    void updateXiaoFeiRank(Player *player);
    void checkXiaoFeiRankInvalid(int8_t connid);
    void checkXiaoFeiRank();
    void sendXiaoFeiRankMail();
    void loadXiaoFeiSumRank();
    void updateXiaoFeiSumRank(Player *player);
    void checkXiaoFeiSumRankInvalid(int8_t connid);
    void checkXiaoFeiSumRank();
    void sendXiaoFeiSumRankMail();
    void loadRechargeSumRank();
    void updateRechargeSumRank(Player *player);
    void checkRechargeSumRankInvalid(int8_t connid);
    void checkRechargeSumRank();
    void sendRechargeSumRankMail();
    void BroadCastMoYuShiJieIconState();
    bool RefreshMysteryShopItem(Player *player);
    void checkRefreshMysteryShop();
    void broadcastRefreshMysteryShop();
    int32_t CalBossScoreAddValue(int32_t nValue);
    int32_t calRechargeBack(int32_t nValue);
    int32_t calRechargeBackHelper(int32_t& nValue);
    void loadCrossRechargeSumRank();
    void updateCrossRechargeSumRank(Player *player);
    void checkCrossRechargeSumRankInvalid(int8_t connid);
    void checkCrossRechargeSumRank();
    void loadCrossXiaoFeiSumRank();
    void updateCrossXiaoFeiSumRank(Player *player);
    void checkCrossXiaoFeiSumRankInvalid(int8_t connid);
    void checkCrossXiaoFeiSumRank();
    void loadCrossChouJiangRank();
    void updateCrossChouJiangRank(Player *player);
    void checkCrossChouJiangRankInvalid(int8_t connid);
    void checkCrossChouJiangRank();
    void loadBossScoreRank();
    void updateBossScoreRank(Player *player);
    void checkBossScoreRankInvalid(int8_t connid);
    void checkBossScoreRank();
    void sendBossScoreRankMail();
    int8_t GetChouJiangType();
    int32_t GetChouJiangSpecialTime();
    int32_t GetScoreCanDrawTime(Player *pPlayer);
    int32_t OnRandScoreDrawItem(Player *pPlayer);

    // Private helpers called from within class
    void OnRecharge(Player* player, int32_t OldValue, int32_t AddValue);
    void AddRechargeRecord(Player* player, int32_t nCount);
    void AddPetIllusionItemRecord(Player* player, int32_t nCount);
    void AddFaBaoValue(Player* player, int32_t nValue);
    int32_t GetFaBaoCritBackItem(Player* player);
    int32_t GetDailyRechargeGift(Player* player);
    int32_t GetRechargeSumGift(Player* player);
    int32_t GetPetIllusionItemGift(Player* player);
    int32_t GetXiaoFeiSumGift(Player* player);
    void GetLianRechargeReward(Player* player);
    int32_t GetHuoYueDuSumGift(Player* player);
    int32_t BuyGiftShopItem(Player* player, int8_t nIndex, int8_t nDay);
    int32_t BuyGiftShopItem2(Player* player, int8_t nIndex);
    void UpdateRank(Player* player, int32_t nType, int32_t nCount, int32_t nTime);
    void UpdateRank(int8_t connid, int8_t nPolicy, int16_t type, int16_t nIndex, const FestivalRank& info) { (void)connid; (void)nPolicy; (void)type; (void)nIndex; (void)info; }
    int32_t GouWuChe(Player* player);
    void BossDie(Unit* unit);
    void BroadcastWorldBossKilled(Unit* unit, Player* killer);
    void AddMoYuShiJieDrop(Player* player, int32_t nCount);
    void AddEquipUpStarBack(Player* player, int32_t nCount);
    void broadcastPlayerAction(Player* player, int32_t nActionId, int32_t nCount);
    void SendDaTiReward(Player* player, int32_t nRank, int32_t nCount);
    int32_t GetEquipQingYiGift(Player* player);
    int32_t GetFriendQingYiGift(Player* player);
    void UpdateMonsterScore(Player* player, int32_t nScore, int32_t nTime);

    static bool isOpen(CFestivalDoubleEleven* _this, FESTIVAL_ACTIVITY_TYPE nType);

private:
    static ShowIcon* getIconState(ShowIcon* retstr, CFestivalDoubleEleven* _this, Player* player);
    static ShowIcon* getWorldBossIconState(ShowIcon* retstr);
    static ShowIcon* getMoYuShiJieIconState(ShowIcon* retstr);
    static void GetIconState(CFestivalDoubleEleven* _this, Player* player, std::list<ShowIcon>* IconList);
    static void GetWorldBossIconState(std::list<ShowIcon>* IconList);
    static void GetMoYuShiJieIconState(std::list<ShowIcon>* IconList);

    void updateDrawRank(int8_t connid, int16_t nIndex, const FestivalRank* info);
    void updateXiaoFeiRank(int8_t connid, int16_t nIndex, const FestivalRank* info, int8_t nDay);
    void updateXiaoFeiSumRank(int8_t connid, int16_t nIndex, const FestivalRank* info);
    void updateRechargeSumRank(int8_t connid, int16_t nIndex, const FestivalRank* info);
    void updateCrossRechargeSumRank(int8_t connid, int16_t nIndex, const FestivalRank* info);
    void updateCrossXiaoFeiSumRank(int8_t connid, int16_t nIndex, const FestivalRank* info);
    void updateCrossChouJiangRank(int8_t connid, int16_t nIndex, const FestivalRank* info);
    void updateBossScoreRank(int8_t connid, int16_t nIndex, const FestivalRank* info);

private:
    int32_t m_ChouJiangCount;
    int8_t m_ChouJiangType;
    int32_t m_GetReardSize;
    std::vector<MemChrBag> m_Items;
    std::vector<LianRechargeCfg> m_LianRechargeCfgVt;
    int32_t m_MapId;
    int32_t m_Mid;
    int32_t m_MonsterId;
    int32_t m_NeedValue;
    int32_t m_Price;
    int32_t m_RechargeDrawSize;
    float m_X;
    float m_Y;
    bool m_bDie;
    Answer::Mutex m_lock;
    std::map<int8_t, std::vector<FestivalRank>> m_mBossScoreRank;
    std::map<int8_t, std::vector<FestivalRank>> m_mCrossChouJiangRank;
    std::map<int8_t, std::vector<FestivalRank>> m_mCrossRechargeSumRank;
    std::map<int8_t, std::vector<FestivalRank>> m_mCrossXiaoFeiSumRank;
    std::map<int8_t, std::vector<FestivalRank>> m_mDrawRank;
    std::map<int, int> m_mEquipUpStarBackStarCount;
    std::map<int8_t, std::vector<FestivalRank>> m_mRechargeSumRank;
    std::map<int8_t, std::vector<std::vector<FestivalRank>>> m_mXiaoFeiRank;
    std::map<int8_t, std::vector<FestivalRank>> m_mXiaoFeiSumRank;
    float m_nBossScoreRate;
    int32_t m_nBuyGiftSize;
    int32_t m_nCrossChouJiangRankSize;
    int32_t m_nCrossRechargeSumRankSize;
    int32_t m_nCrossXiaoFeiSumRankSize;
    int32_t m_nDailyLimitShop2Size;
    int32_t m_nDailyLimitShopCostType;
    int32_t m_nDailyLimitShopCostValue;
    int32_t m_nDailyLimitShopLimit;
    int32_t m_nDailyRechargeDrawLimit;
    int32_t m_nDailyRechargeDrawSize;
    int32_t m_nDailyRechargeSize;
    int32_t m_nDay;
    int64_t m_nDrawLoopTimes;
    int32_t m_nDrawRankLimit;
    int32_t m_nDrawRankSize;
    int32_t m_nDropGroup;
    int32_t m_nEndDay;
    int32_t m_nEquipUpStarBackId;
    int32_t m_nFriendQingYiSize;
    int32_t m_nGameStartDay;
    int32_t m_nGiftShopSize;
    int32_t m_nHotSign;
    int32_t m_nHuoYueDuSumSize;
    int32_t m_nIcon;
    int32_t m_nIconHideDay;
    int32_t m_nMaxCount;
    int32_t m_nMinute;
    int32_t m_nMysteryShopRefreshBroad;
    int32_t m_nOnlineEndMinute;
    int32_t m_nOnlineStartMinute;
    int64_t m_nOnlineTimeArea;
    int32_t m_nPetIllusionItemSize;
    int32_t m_nRechargeBackSize;
    int32_t m_nRechargeDrawValue;
    int32_t m_nRechargeSumRankSize;
    int32_t m_nRechargeSumSize;
    int32_t m_nRewardSize;
    int32_t m_nScoreSize;
    int32_t m_nSize;
    int32_t m_nStartDay;
    int32_t m_nUniteStartDay;
    int32_t m_nVersion;
    int32_t m_nWishRecharge;
    int32_t m_nWorldBossEndBroadcast;
    int32_t m_nWorldBossStartBroadcast;
    int32_t m_nWorldBossIcon;
    int32_t m_nWorldBossKillBroadcast;
    int32_t m_nXiaoFeiDrawSize;
    int32_t m_nXiaoFeiDrawValue;
    int32_t m_nXiaoFeiRankSize;
    int32_t m_nXiaoFeiSumRankLimit;
    int32_t m_nXiaoFeiSumRankSize;
    int32_t m_nXiaoFeiSumSize;
    MemChrBag m_sMysteryShopRefreshCost;
    std::vector<RateItem> m_vBossScoreDrawReward;
    std::vector<int> m_vBossScoreRankMail;
    std::vector<std::vector<MemChrBag>> m_vBossScoreRankReward;
    std::vector<std::vector<MemChrBag>> m_vBossScoreReward;
    std::vector<int> m_vBossScoreiLimit;
    std::vector<int> m_vBuyGiftBroadcast;
    std::vector<std::vector<MemChrBag>> m_vBuyGiftItem;
    std::vector<int> m_vBuyGiftPrice;
    std::vector<int> m_vCrossChouJiangRankLimit;
    std::vector<int> m_vCrossChouJiangRankMail;
    std::vector<std::vector<MemChrBag>> m_vCrossChouJiangRankReward;
    std::vector<int> m_vCrossRechargeSumRankLimit;
    std::vector<int> m_vCrossRechargeSumRankMail;
    std::vector<std::vector<MemChrBag>> m_vCrossRechargeSumRankReward;
    std::vector<int> m_vCrossXiaoFeiSumRankLimit;
    std::vector<int> m_vCrossXiaoFeiSumRankMail;
    std::vector<std::vector<MemChrBag>> m_vCrossXiaoFeiSumRankReward;
    std::vector<int> m_vDaTiRankMail;
    std::vector<std::vector<MemChrBag>> m_vDaTiRankReward;
    std::vector<int> m_vDailyLimitShop2Broadcast;
    std::vector<ShopGoods> m_vDailyLimitShop2Goods;
    std::vector<MemChrBag> m_vDailyLimitShopItem;
    std::vector<int> m_vDailyRechargeBroad;
    std::vector<int> m_vDailyRechargeLimit;
    std::vector<std::vector<MemChrBag>> m_vDailyRechargeReward;
    std::vector<MemChrBag> m_vDrawLoopReward;
    std::vector<int> m_vDrawRankMail;
    std::vector<std::vector<MemChrBag>> m_vDrawRankReward;
    std::vector<int> m_vEndDay;
    std::vector<int> m_vEquipQingYiLimit;
    std::vector<std::vector<MemChrBag>> m_vEquipQingYiReward;
    std::vector<int> m_vFriendQingYiLimit;
    std::vector<std::vector<MemChrBag>> m_vFriendQingYiReward;
    std::vector<int> m_vGetBossScoreMail;
    std::vector<std::vector<ShopGoods>> m_vGiftShopGoods;
    std::vector<int> m_vGouWuCheGiftPrice;
    std::vector<MemChrBag> m_vGouWuCheItem;
    std::vector<int> m_vHuoYueDuSumLimit;
    std::vector<std::vector<MemChrBag>> m_vHuoYueDuSumReward;
    std::vector<std::vector<MemChrBag>> m_vLandGift;
    std::vector<std::vector<MemChrBag>> m_vLandSumGift;
    std::vector<std::vector<MemChrBag>> m_vMoYuShiJieDrop;
    std::vector<MemChrBag> m_vOnlineReward;
    std::vector<int8_t> m_vOpen;
    std::vector<int> m_vPetIllusionItemLimit;
    std::vector<std::vector<MemChrBag>> m_vPetIllusionItemReward;
    std::vector<int> m_vRechargeBackLimit;
    std::vector<int> m_vRechargeBackValue;
    std::vector<int> m_vRechargeSumBroadcast;
    std::vector<int> m_vRechargeSumLimit;
    std::vector<int> m_vRechargeSumRankLimit;
    std::vector<int> m_vRechargeSumRankMail;
    std::vector<std::vector<MemChrBag>> m_vRechargeSumRankReward;
    std::vector<std::vector<MemChrBag>> m_vRechargeSumReward;
    std::vector<int> m_vScoreLimit;
    std::vector<int> m_vStartDay;
    std::vector<MemChrBag> m_vWishReward;
    std::vector<TimeArea> m_vWorldBossMinute;
    std::vector<int> m_vXiaoFeiRankLimit;
    std::vector<int> m_vXiaoFeiRankMail;
    std::vector<std::vector<MemChrBag>> m_vXiaoFeiRankReward;
    std::vector<int> m_vXiaoFeiSumBroadcast;
    std::vector<int> m_vXiaoFeiSumLimit;
    std::vector<int> m_vXiaoFeiSumRankMail;
    std::vector<std::vector<MemChrBag>> m_vXiaoFeiSumRankReward;
    std::vector<std::vector<MemChrBag>> m_vXiaoFeiSumReward;
    std::vector<MemChrBag> m_BackItem;
    std::vector<int> m_nDailyRechargeDrawLimitVec;
    std::vector<int> m_nRechargeDrawValueVec;
    std::vector<int> m_nXiaoFeiDrawValueVec;
};

#endif // _CFESTIVALDOUBLEELEVEN_H_