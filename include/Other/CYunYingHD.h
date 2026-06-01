#ifndef _CYUNYINGHD_H_
#define _CYUNYINGHD_H_

#include "Character/CExtSystemBase.h"

class CYunYingHD : public CExtSystemBase
{
public:
    CYunYingHD();
    virtual ~CYunYingHD();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnGetShouChongReward(Answer::NetPacket* inPacket);
    int32_t OnGetBuyTeHuiItem(Answer::NetPacket* inPacket);
    int32_t OnGetEveryDayChongZhiGift(Answer::NetPacket* inPacket);
    int32_t OnGetTotalChongZhi(Answer::NetPacket* inPacket);
    int8_t GetShouChongIconState();
    int8_t GetShouChongState(int8_t nIndex);
    void SendShouChongInfo();
    void SendShouChongIcon();
    void GetShouChongIconState(IconStateList* IconList);
    void GetShouChongIconStu(ShowIcon* __return_ptr retstr);
    void SendTeHuiInfo();
    int32_t getTeHuiLimitTime();
    void checkTeHuiTime();
    void SendTeHuiIcon();
    void GetTeHuiIconState(IconStateList* IconList);
    void GetTeHuiIconStu(ShowIcon* __return_ptr retstr);
    bool IsHaveTeHuiGift();
    void SendEveryDayChongZhiInfo(int8_t nType);
    void SendEveryDayChongZhiIcon(int8_t nType);
    bool CanShowEveryChongZhiIcon();
    void GetEveryDayChongZhiIconState(IconStateList* IconList);
    int32_t getEveryDayChongZhiIcon(int8_t nType);
    bool HaveEveryDayChongZhiGiftCount(int8_t nType, int32_t* Count);
    void GongGao(int32_t GongGaoId);
    int32_t HaveTotalChongZhiRewardCount();
    void SendTotalChongZhiIcon();
    void SendTotalChongZhiInfo();
    void GetTotalChongZhiIconState(IconStateList* IconList);
    bool AllGetTotalChongZhiReward();
    void GetTotalChongZhiIconStu(ShowIcon* __return_ptr retstr);
    void AddTotalChongZhiCount(int32_t AddCount);
    int32_t OnGetMobilePhoneGift(Answer::NetPacket* inPacket);
    int32_t OnDBGetMobilePhoneGift(Answer::NetPacket* inPacket);
    void GetMobilePhoneGiftIconState(IconStateList* IconList);
    void SendMobilePhoneGiftIcon();
    void GetAdultGiftIconState(IconStateList* IconList);
    void SendAdultGiftIcon();
    void GetAdultGiftIconStu(ShowIcon* __return_ptr retstr, int32_t nIcon);
    void SuperMemberRecharge(int32_t nGold);
    void GetSuperMemberIconState(IconStateList* IconList);
    int32_t OnGetZeroBuyPetGift(Answer::NetPacket* inPacket);
    void OnZeroBuyPetOpen();
    void SendZeroBuyPetIcon();
    void GetZeroBuyPetIconState(IconStateList* IconList);
    void GetZeroBuyPetIconStu(ShowIcon* __return_ptr retstr);
};

#endif // _CYUNYINGHD_H_


