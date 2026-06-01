#ifndef _CKIAFURECHARGE_H_
#define _CKIAFURECHARGE_H_

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

class CKiaFuRecharge
{
public:
    CKiaFuRecharge();
    ~CKiaFuRecharge();

    void Init(int32_t line);
    void OnDaySwitch();
    void OnGetRechargeSumReward(Player *player, int32_t nIndex);
    void OnGetChouJiangReward(Player *player, int32_t nIndex);
    void OnGetXiaoFeiReward(Player *player, int32_t nIndex);
    void OnXiaoFeiSum(Player *player, int32_t AddTimes);
    void OnRecharge(Player *player, int32_t OldValue, int32_t AddValue);
    void OnAddChouJiangTimes(Player *player, int32_t AddTimes);
    void SendChouJiangIconState(Player *player);
    bool needShowShouJinagIcon();
    int32_t GetChouJiangLeftTime();
    bool IsInTime(KAI_FU_RECHARGE_TYPE nType);
    void initCfgData();
    bool IsInTime();
    void GetIconState(Player *player, IconStateList * IconList);
    void SendIconState(Player *player);
    bool needShowIcon();
    void hideIcon(int32_t nIconId);
    int32_t getLeftTime();
    void GongGao(int32_t GongGaoId, Player *player);
    int32_t HaveRechargeRewardCount(Player *player);
    int32_t HaveChouJiangCount(Player *player);

private:
        // TODO: 确认类型 m_ChouJiangIcon
        const CfgActivity* m_LianRechargeCfgVt;  // TODO: 确认配置类型
        // TODO: 确认类型 m_nDay
        // TODO: 确认类型 m_nEndDay
        // TODO: 确认类型 m_nIcon
        // TODO: 确认类型 m_nMinute
        // TODO: 确认类型 m_nSize
        // TODO: 确认类型 m_nValues
        // TODO: 确认类型 m_nXiaoFeiSize
        // TODO: 确认类型 m_vEndDay
        // TODO: 确认类型 m_vStartDay
        // TODO: 确认类型 m_vSumGift
        // TODO: 确认类型 m_vSumGolds
        int32_t m_vXiaoFeiGongGaoId;
        // TODO: 确认类型 m_vXiaoFeiSumGift
};

#endif // _CKIAFURECHARGE_H_
