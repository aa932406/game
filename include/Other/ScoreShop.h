#ifndef _SCORESHOP_H_
#define _SCORESHOP_H_

#include "Character/CExtSystemBase.h"

class ScoreShop : public CExtSystemBase
{
public:
    ScoreShop();
    virtual ~ScoreShop();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnBuyItem(Answer::NetPacket* inPacket);
    void AddServerRecord(MemChrBag* Item);
    void SendLimitInfo(int32_t Index);
    int32_t GetLimitCount(int32_t index);
    void AddLimitCount(int32_t index, int32_t count);
};

#endif // _SCORESHOP_H_

