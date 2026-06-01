#ifndef _CUNITESERVER_H_
#define _CUNITESERVER_H_

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

class CUniteServer
{
public:
    CUniteServer();
    ~CUniteServer();

    void OnDaySwitch();
    void OnNewMinute(int32_t nMinute);
    int32_t GetEndCollectDropTime();
    bool RefreshMysteryShopItem(Player *player);
    void Init(int32_t line);
    void OnGetChouJiangReward(Player *player, int32_t nIndex);
    void OnAddChouJiangTimes(Player *player, int32_t AddTimes);
    void OnRecharge(Player *player, int32_t AddValue);
    void GongGao(int32_t GongGaoId, Player *player);
    void AddHuoYueDu(Player *player, int32_t Values);
    void AddXiaoFeiValue(Player *player, int32_t Value);
    void AddWingLevelUp(Player *player, int32_t Value);
    int32_t GetCanXiaoFeiDrawTime(Player *player);
    int32_t GetWingLevelUpRewardCount(Player *player);
    void GetIconState(Player *player, IconStateList * IconList);
    void GetSpiderQueenIconState(IconStateList * IconList);
    void SendIconState(Player *player);
    void SendSpiderQueenIconState();
    void getIconState(ShowIcon &retstr, Player *player);
    void getSpiderQueenIconState(ShowIcon &retstr);
    void SendUniteServerInfo(Player *player);
    bool IsInUnitServerTime();
    bool IsInTime(Unit_Server_Activity_typ nType);
    int32_t GetRechargeGift(Player *player, int32_t nIndex);
    int32_t BuyDiscountGift(Player *player, int16_t nIndex);
    int32_t GetHuoYueDuGift(Player *player, int32_t nIndex);
    int32_t BuyChangeNameCard(Player *player);
    int32_t GetWingLevelUpGift(Player *player, int32_t nIndex);
    bool IsInSpiderQueenTime();
    void BossDie(int32_t MonsterId, std::string *p_name, CharId_t Cid);
    void checkSpiderQueen();
    void broadcastSpiderQueenStart();
    void broadcastSpiderQueenEnd();
    void appendRechargeInfo(Answer::NetPacket *packet, Player *player);
    void appendDiscountGiftInfo(Answer::NetPacket *packet, Player *player);
    void appendHuoYueDuInfo(Answer::NetPacket *packet, Player *player);
    int32_t getLeftTime();
    void checkMysteryShopRefreshBroad();
    int32_t GetChouJiangType();
    int32_t GetChouJiangSpecialTime();

private:
        int32_t m_ChouJiangCount;
        // TODO: 确认类型 m_ChouJiangType
        // TODO: 确认类型 m_Item
        // TODO: 确认类型 m_Items
        const CfgActivity* m_LianRechargeCfgVt;  // TODO: 确认配置类型
        int32_t m_MapId;
        // TODO: 确认类型 m_Mid
        int32_t m_MonsterId;
        int32_t m_UseItemCount;
        // TODO: 确认类型 m_X
        // TODO: 确认类型 m_XiaoFeiDrawSize
        // TODO: 确认类型 m_Y
        // TODO: 确认类型 m_bDie
        // TODO: 确认类型 m_nDay
        // TODO: 确认类型 m_nEndDay
        // TODO: 确认类型 m_nHuoYueSize
        // TODO: 确认类型 m_nIcon
        // TODO: 确认类型 m_nMinute
        // TODO: 确认类型 m_nMysteryShopRefreshBroad
        // TODO: 确认类型 m_nPrice
        // TODO: 确认类型 m_nRechargeDaysSize
        // TODO: 确认类型 m_nSize
        // TODO: 确认类型 m_nSpiderQueenEndBroadcast
        // TODO: 确认类型 m_nSpiderQueenIcon
        // TODO: 确认类型 m_nSpiderQueenStartBroadcast
        // TODO: 确认类型 m_nStartDay
        // TODO: 确认类型 m_nXiaoFeiDrawValue
        // TODO: 确认类型 m_sMysteryShopRefreshCost
        // TODO: 确认类型 m_vEndDay
        // TODO: 确认类型 m_vHuoYueGift
        // TODO: 确认类型 m_vHuoYueValue
        // TODO: 确认类型 m_vRechargeGift
        // TODO: 确认类型 m_vRechargeValue
        // TODO: 确认类型 m_vSpiderQueenMinute
        // TODO: 确认类型 m_vStartDay
};

#endif // _CUNITESERVER_H_
