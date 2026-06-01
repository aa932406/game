#ifndef _CSEVENDAYTASK_H_
#define _CSEVENDAYTASK_H_

#include "Character/CExtSystemBase.h"

class CSevenDayTask : public CExtSystemBase
{
public:
    CSevenDayTask();
    virtual ~CSevenDayTask();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t OnAskReward(Answer::NetPacket* inPacket);
    int32_t OnAskSumReward(Answer::NetPacket* inPacket);
    void UpdateTaskState(int32_t nType, int32_t Values, bool NeedSend);
    void SendTaskSTate();
    void OpenSevenDayTask();
    void GetIcon(IconStateList* IconList);
    void SendIcon();
    int32_t HaveRewardCount();
    void GetIcon(ShowIcon* __return_ptr retstr);
};

#endif // _CSEVENDAYTASK_H_


