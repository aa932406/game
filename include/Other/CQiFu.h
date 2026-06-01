#ifndef _CQIFU_H_
#define _CQIFU_H_

#include "Character/CExtSystemBase.h"

class CQiFu : public CExtSystemBase
{
public:
    CQiFu();
    virtual ~CQiFu();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    bool CheckQiFuType(int8_t Type);
    int32_t OnQiFu(Answer::NetPacket* inPacket);
    void SendQiFuSuccess(int8_t Type, int32_t AddCount, int8_t Double);
    void SendQiFuInfo();
    void GetQiFuStu(IconStateList* IconList);
    void GetShowIconStu(ShowIcon* __return_ptr retstr);
    void SendQIFuIcon();
};

#endif // _CQIFU_H_


