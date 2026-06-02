#ifndef _CZONGHEYUNYINGHD_H_
#define _CZONGHEYUNYINGHD_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

enum ZONG_HE_YUN_YING_HD_TYPE : int32_t
{
    ZONG_HE_YUN_YING_HD_NONE      = 0,
    ZONG_HE_YUN_YING_HD_RECHARGE  = 1,
    ZONG_HE_YUN_YING_HD_CONSUME   = 2,
    ZONG_HE_YUN_YING_HD_OTHER     = 3,
};

class Player;
class Map;
class Unit;
class Npc;
class CActivity;
class RankInfo;
class RechargeDailyRank;
struct ShowIcon;

class CZongHeYunYingHD
{
public:
    CZongHeYunYingHD();
    ~CZongHeYunYingHD();

    void OnDaySwitch();
    void OnNewMinute(int32_t nMinute);
    void Init(int32_t line);
    void initOpenList(std::string * str);
    void initCfgData();
    void loadRankData();
    void SendIconState(Player *player);
    void hideIcon(int32_t nIconId);
    void SendActivityInfo(Player *player);
    void OnRecharge(Player *player, int32_t nValue);
    void checkRankInvalid(int8_t connid, int8_t nType);
    int32_t GetRechargeDailyReward(Player *player);
    int32_t BuyOnceShopItem(Player *player, int32_t nIndex);
    bool IsInTime();
    bool needShowIcon();
    bool IsInTime(ZONG_HE_YUN_YING_HD_TYPE nType);
    bool IsOpen(ZONG_HE_YUN_YING_HD_TYPE nType);
    int32_t getLeftTime();
    void sendRechargeDailyRankReward();
    int32_t getRechargePlayer(int8_t connid);
    void getIconState(ShowIcon &retstr, Player *player);
    void UpdatePlayerInfo(int32_t id, Answer::NetPacket *inPacket);
    void UpdateRankInfo(int32_t id, int32_t nValue, int8_t connid, RankInfo *info, RechargeDailyRank *vRank);
    void UpdateRankInfo(int8_t connid, int8_t nType, int16_t nDay, const std::vector<class FestivalRank>& vRank) { (void)connid; (void)nType; (void)nDay; (void)vRank; }
    int32_t GetRechargeTeamShopDailyReward(Player *player, int32_t nIndex);

private:
        bool isOpen(ZONG_HE_YUN_YING_HD_TYPE nType);
        void updateRankInfo(int8_t connid, int8_t nType, RankInfo *info);
        void sendLineSocialPlayerInfo(Player *player, int32_t nType, RankInfo *info);
        // TODO: 确认类型 m_lock
        // TODO: 确认类型 m_mRechargeDailyPlayer
        // TODO: 确认类型 m_mRechargeDailyRank
        // TODO: 确认类型 m_nDay
        // TODO: 确认类型 m_nEndDay
        // TODO: 确认类型 m_nIcon
        // TODO: 确认类型 m_nMinute
        // TODO: 确认类型 m_nOnceShopSize
        // TODO: 确认类型 m_nRechargeDailyRewardLimit
        // TODO: 确认类型 m_nRechargeDailyValue
        // TODO: 确认类型 m_nRechargeRankDailyArea
        // TODO: 确认类型 m_nRechargeRankDailySize
        // TODO: 确认类型 m_nRechargeTeamShopDailySize
        // TODO: 确认类型 m_nStartDay
        // TODO: 确认类型 m_vEndDay
        // TODO: 确认类型 m_vOnceShopItem
        // TODO: 确认类型 m_vOnceShopPrice
        // TODO: 确认类型 m_vOpen
        // TODO: 确认类型 m_vRechargeDailyReward
        int32_t m_vRechargeRankDailyIndex;
        // TODO: 确认类型 m_vRechargeRankDailyMail
        // TODO: 确认类型 m_vRechargeRankDailyReward
        // TODO: 确认类型 m_vRechargeTeamShopDailyPlayer
        // TODO: 确认类型 m_vRechargeTeamShopDailyReward
        // TODO: 确认类型 m_vRechargeTeamShopDailyValue
        // TODO: 确认类型 m_vStartDay
};

#endif // _CZONGHEYUNYINGHD_H_
