#ifndef _CSHENWEI_H_
#define _CSHENWEI_H_

#include "Character/CExtSystemBase.h"

class CShenWei : public CExtSystemBase
{
public:
    CShenWei();
    virtual ~CShenWei();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void AddCharAttr();
    int32_t GetRankAttr(int64_t Exp);
    int32_t GetShenWeiLevel();
};

#endif // _CSHENWEI_H_

