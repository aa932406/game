#ifndef _CGONGMING_H_
#define _CGONGMING_H_

#include "Character/CExtSystemBase.h"

class CGongMing : public CExtSystemBase
{
public:
    CGongMing();
    virtual ~CGongMing();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void AddCharAttr();
    void AddGongMingValues(int32_t Values);
    int32_t OnGongMinLevelUp(Answer::NetPacket* inPacket);
    void SendGongMinInfo();
};

#endif // _CGONGMING_H_

