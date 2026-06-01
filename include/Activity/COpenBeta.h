#ifndef _COPENBETA_H_
#define _COPENBETA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;

class COpenBeta
{
public:
    COpenBeta();
    ~COpenBeta();

    void Init(int32_t line);
    int32_t GetFlopType(int32_t nFlopId);
    bool CanFlopDraw(Player *pPlayer, int32_t FlopId);
    int32_t GetMaxFlopTimes(Player *pPlayer);
    void OnDaySwitch();
    void OnNewMinute(int32_t nMinute);
    void updateStartTime();
    void GetIconState(Player *player, IconStateList * IconList);
    void SendIconState(Player *player);
    void getIconState(ShowIcon &retstr, Player *player);
    void SendActivityInfo(Player *player);
    void GetBossIconState(IconStateList * IconList);
    void SendBossIconState();
    void getBossIconState(ShowIcon &retstr);
    bool IsInBossTime();
    void BossDie(int32_t MonsterId, std::string *p_name, CharId_t Cid);
    void BroadcastBossKilled(std::string * name, CharId_t cid);
    void CheckBoss();
    void broadcastBossStart();
    void broadcastBossEnd();
    int32_t GetRechargeGift(Player *player, int32_t nIndex);
    int32_t BuyGiftShopItem(Player *player, int8_t nIndex);
    int32_t BuyTitleShopItem(Player *player, int8_t nIndex);
    int32_t BuyShiZhuangShopItem(Player *player, int8_t nIndex);
    int32_t BuyLiQuan(Player *player, int8_t nIndex);
    void AddOnlineRecord(Player *player);
    bool canGetOnlineGift(Player *player);
    int32_t GetOnlineTimeRewardTime(Player *player);
    int32_t GetOnlineGift(Player *player);
    bool IsInTime(int8_t Type);
    int32_t getLeftTime();
    bool IsOpenBetaTime();
    int32_t GetEndCollectDropTime();
    int32_t GetChouJiangType();
    void LoadYanHuaValue();
    void UpdateYanHuaValue();
    void AddYanHuaPoint(int32_t Values);
    int32_t GetYanHuaPointReward(Player *player, int8_t nIndex);
    int32_t GetQianDaoReward(Player *player, int8_t nIndex, int8_t nType);
    void loadXiaoFeiSumRank();
    void updateXiaoFeiSumRank(Player *player, int32_t AddValues);
    void checkXiaoFeiSumRankInvalid(int8_t connid);
    void checkXiaoFeiSumRank();
    void sendXiaoFeiSumRankMail();
    void SendResult(Player *pPlayer, int8_t Type, int32_t Index, int32_t param);

private:
        // TODO: 确认类型 m_BuQianPrice
        // TODO: 确认类型 m_ChouJiangType
        // TODO: 确认类型 m_CurrYanHuaPoint
        // TODO: 确认类型 m_FlopCost
        // TODO: 确认类型 m_FlopSize
        // TODO: 确认类型 m_FlopType
        // TODO: 确认类型 m_ItemDataVector
        // TODO: 确认类型 m_LiQuanItem
        int32_t m_LiQuanLimitCount;
        // TODO: 确认类型 m_LiQuanPriceVt
        // TODO: 确认类型 m_LiQuanSize
        int32_t m_MapId;
        // TODO: 确认类型 m_Mid
        int32_t m_MonsterId;
        // TODO: 确认类型 m_QianDaoReward
        // TODO: 确认类型 m_QianDaoSize
        // TODO: 确认类型 m_X
        // TODO: 确认类型 m_Y
        // TODO: 确认类型 m_YanHuaPointVt
        // TODO: 确认类型 m_YanHuaReward
        // TODO: 确认类型 m_YanHuaSize
        // TODO: 确认类型 m_bDie
        // TODO: 确认类型 m_lock
        // TODO: 确认类型 m_mXiaoFeiSumRank
        // TODO: 确认类型 m_nBossEndBroadcast
        // TODO: 确认类型 m_nBossIcon
        // TODO: 确认类型 m_nBossKillBroadcast
        // TODO: 确认类型 m_nBossStartBroadcast
        // TODO: 确认类型 m_nDailyLimitShopSize
        // TODO: 确认类型 m_nDay
        // TODO: 确认类型 m_nEndDay
        // TODO: 确认类型 m_nIcon
        int32_t m_nMaxCount;
        // TODO: 确认类型 m_nMinute
        // TODO: 确认类型 m_nOnlineEndMinute
        // TODO: 确认类型 m_nOnlineStartMinute
        int64_t m_nOnlineTimeArea;
        // TODO: 确认类型 m_nRechargeDaysSize
        // TODO: 确认类型 m_nShiZhuangShopSize
        // TODO: 确认类型 m_nShowSize
        // TODO: 确认类型 m_nStartDay
        // TODO: 确认类型 m_nTitleShopSize
        // TODO: 确认类型 m_nXiaoFeiSumRankLimit
        // TODO: 确认类型 m_nXiaoFeiSumRankSize
        // TODO: 确认类型 m_vBossMinute
        // TODO: 确认类型 m_vDailyLimitShopBroadcast
        // TODO: 确认类型 m_vDailyLimitShopGoods
        // TODO: 确认类型 m_vEndDay
        // TODO: 确认类型 m_vOnlineReward
        // TODO: 确认类型 m_vRechargeGift
        // TODO: 确认类型 m_vRechargeValue
        // TODO: 确认类型 m_vShiZhuangShopBroadcast
        // TODO: 确认类型 m_vShizhuangShopGoods
        // TODO: 确认类型 m_vStartDay
        // TODO: 确认类型 m_vTitleShopBroadcast
        // TODO: 确认类型 m_vTitleShopGoods
        // TODO: 确认类型 m_vXiaoFeiSumRankMail
        // TODO: 确认类型 m_vXiaoFeiSumRankReward
};

#endif // _COPENBETA_H_
