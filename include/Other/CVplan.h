#ifndef _CVPLAN_H_
#define _CVPLAN_H_

#include "Character/CExtSystemBase.h"

struct ShowIcon;

class CVplan : public CExtSystemBase
{
public:
    CVplan();
    virtual ~CVplan();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void OnWeekSwitch(int32_t nDiffWeeks);
    int32_t OnLoginFromYYgameApp(Answer::NetPacket* inPacket);
    void RefreshWeekTime(int32_t nDiffWeeks);
    void AddAttr();
    int32_t GetExpRatio();
    int32_t OnGetReward(Answer::NetPacket* inPacket);
    bool HaveEveryDayGift();
    bool HaveLevelGift();
    bool HaveVplanGift();
    void SendVplanInfo();
    void SendVplanIcon();
    void GetVplanIconState(IconStateList* IconList);
    void GetVplanIconStu(ShowIcon* retstr);
    int32_t OnGetYYLevelReward(Answer::NetPacket* inPacket);
    int32_t HaveYYLevelReward();
    void SendYYInfo();
    int32_t OnGetYYVipGift(Answer::NetPacket* inPacket);
    void SendYYVipInfo();
    void GetYYVipIconState(IconStateList* IconList);
    void SendYYVipIcon();
    void GetYYVipIconStu(ShowIcon* retstr);
    int32_t HaveYYVipLeftGift();
    void AppendYYInfo(Answer::NetPacket* packet);
    int32_t OnGetSWVipReward(Answer::NetPacket* inPacket);
    int32_t OnGetSWVipBarReward(Answer::NetPacket* inPacket);
    void SendSwVipReward();
    void AppendSwVipInfo(Answer::NetPacket* packet);
    void CheckSwBarLoginCount();
    void GetSwVipIconState(IconStateList* IconList);
    void GetSwVipBarIconState(IconStateList* IconList);
    void sendSwVipIcon();
    void getSwVipIconStu(ShowIcon* retstr);
    int32_t getSwVipRewardCount();
    bool isGetAllSwVipReward();
    bool checkSwVipRewardCondition(int8_t nType, int32_t nCondition);
    void resetSwVipDailyReward();
    void sendSwVipBarIcon();
    void getSwVipBarIconStu(ShowIcon* retstr);
    int32_t getSwVipBarRewardCount();
    bool isGetAllSwVipBarReward();
    bool checkSwBarRewardCondition(int8_t nType, int32_t nCondition);
    void resetSwBarDailyReward();
    int32_t OnGetSgGameApp(Answer::NetPacket* inPacket);
    int32_t OnGetXunLeiReward(Answer::NetPacket* inPacket);
    void SendXunLieInfo();
    void SendSgGameAppInfo();
    int32_t HaveSgGameAppReward();
    bool HaveSgDownLoadGift();
    void getSgDownLoadIconStu(ShowIcon* retstr);
    void SendSgDownLoadIcon();
    void GetSgDownLoadIocnState(IconStateList* IconList);
    void GetXunLeiIconState(IconStateList* IconList);
    void SendXunLeiIcon();
    void GetXunLeiDownlondIcon(ShowIcon* retstr);
    void GetXunLeiDaTingIcon(ShowIcon* retstr);
    void GetXunLeiVipIcon(ShowIcon* retstr);
    bool IsGetAllXunLeiReward(int32_t Type);
    int32_t HaveXunLeiVipReward();
    int32_t OnGetLuDaShiVipGift(Answer::NetPacket* inPacket);
    void SendLuDaShiVipInfo();
    int32_t HaveLuDaShiVipLeftGift();
    void GetLuDaShiStu(ShowIcon* retstr);
    void GetLuDaShiIconState(IconStateList* IconList);
    void SendLuDaShiIcon();
};

#endif // _CVPLAN_H_


