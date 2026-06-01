#ifndef _CTIANLING_H_
#define _CTIANLING_H_

#include "Character/CExtSystemBase.h"

class CTianLing : public CExtSystemBase
{
public:
    CTianLing();
    virtual ~CTianLing();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void InitSystem() override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void InitTianLing();
    void AddCharAttr();
    int32_t OnTianLingLevelUp(Answer::NetPacket* inPacket);
    void InitZhanHun();
    int32_t OnZhanHunLevelUp(Answer::NetPacket* inPacket);
    void InitSunAndMoon();
    int32_t OnSunAndMoonLevelUp(Answer::NetPacket* inPacket);
    bool GetMonsterItem(MemChrBag item);
};

#endif // _CTIANLING_H_

