#ifndef _CLEVELREFINING_H_
#define _CLEVELREFINING_H_

#include "Character/CExtSystemBase.h"

class CLevelRefining : public CExtSystemBase
{
public:
    CLevelRefining();
    virtual ~CLevelRefining();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnRefining(Answer::NetPacket* inPacket);
    void SendRefinInfo();
    int32_t GetLeftTime();
    void GetIcon(IconStateList* IconList);
    void GetIconStu(ShowIcon* __return_ptr retstr);
    void SendIcon();
};

#endif // _CLEVELREFINING_H_
