#ifndef _CKAIFUHUODONG_H_
#define _CKAIFUHUODONG_H_

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

class CKaiFuHuoDong
{
public:
    CKaiFuHuoDong();
    ~CKaiFuHuoDong();

    void Init(int32_t line);
    void GetKaiFuHuoDongData(int32_t Index);
    bool InTime();
    int32_t OnGetReward(Player *pPlayer, Answer::NetPacket *inPacket);
    int32_t GetLimitCount(int32_t index);
    void AddLimitCount(int32_t index, int32_t Values);
    void GongGao(Player *pPlayer, int32_t GongGaoId, int32_t Index);
    void AddRewardRecord(Player *pPlayer, int32_t Index);
    bool IsAlreadyGet(Player *pPlayer, int32_t Index);
    void SendHuoDongInfo(Player *pPlayer);
    int32_t AskInfo(Player *pPlayer, Answer::NetPacket *inPacket);
    int32_t GetDaysFromServerStart();
    void SendKaiFuHuoDongIcon(Player *pPlayer);
    int32_t HaveRewardCount(Player *pPlayer);
    void OnKilledMonster(Player *pPlayer, int32_t Mid);
    void SendBossFirstKillInfo(Player *pPlayer);
    bool HaveTeHuiGift(Player *pPlayer);
    bool HaveBuyPetItem(Player *pPlayer);
    void GetKaiFuPetIcon(Player *pPlayer, IconStateList * IconList);
    void SendKaiFuPetIcon(Player *pPlayer);
    int32_t GetKaiFuPetStartDay();
    int32_t GetKaiFuPetEndDay();
    int32_t GetFlopType(int32_t nFlopId);
    bool CanFlopDraw(Player *pPlayer, int32_t FlopId);
    int32_t GetMaxFlopTimes(Player *pPlayer);

private:
        // TODO: 确认类型 m_BossFirstKilledMap
        int64_t m_FlopBuyTimes;
        // TODO: 确认类型 m_FlopCost
        int64_t m_FlopEndTime;
        int64_t m_FlopFreeTimes;
        // TODO: 确认类型 m_FlopSize
        int64_t m_FlopStartTime;
        // TODO: 确认类型 m_FlopType
        const CfgActivity* m_KaiFuHuoDongCfg;  // TODO: 确认配置类型
        // TODO: 确认类型 m_Lock
        // TODO: 确认类型 m_nHDLastDay
};

#endif // _CKAIFUHUODONG_H_
