#ifndef _CTESTSERVERREWARD_H_
#define _CTESTSERVERREWARD_H_

#include "Character/CExtSystemBase.h"

class CTestServerReward : public CExtSystemBase
{
public:
    CTestServerReward();
    virtual ~CTestServerReward();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnGetTestServerReward(Answer::NetPacket* inPacket);
    void GetTestServerIcon(IconStateList* IconList);
    void SendTestServerIcon();
    void GetTestServerIconStu(ShowIcon* retstr);
    int32_t CanActivationCount();
    bool IsAllGetReward();
};

#endif // _CTESTSERVERREWARD_H_

