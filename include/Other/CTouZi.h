#ifndef _CTOUZI_H_
#define _CTOUZI_H_

#include "Character/CExtSystemBase.h"

class CTouZi : public CExtSystemBase
{
public:
    CTouZi();
    virtual ~CTouZi();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnTouZi(Answer::NetPacket* inPacket);
    int32_t OnGetTouZiReward(Answer::NetPacket* inPacket);
    int32_t OnAskTouZiInfo(Answer::NetPacket* inPacket);
    int32_t GetSevenTouZiReward(int16_t nIndex);
    int32_t GetMonthTouZiReward(int16_t nIndex);
    void SendTouZiInfo();
    void GetTouZiIconState(IconStateList* IconList);
    void SendTouZiIcon();
    bool IsAllGetSevenDayTouZi();
    bool IsAllGetMonthTouZi();
    int32_t GetSevenDayRewardCount();
    int32_t GetMonthRewardCount();
    void GetTouZiIconStu(ShowIcon* __return_ptr retstr);

private:
    int32_t m_SevenDayTouZiTime;
    int32_t m_SevenDayRecord;
    int32_t m_MonthTouZiTime;
    int32_t m_MonthTouZiRecord;
};

#endif // _CTOUZI_H_


