#ifndef _CGOBLIN_H_
#define _CGOBLIN_H_

#include "Character/CExtSystemBase.h"

class CGoblin : public CExtSystemBase
{
public:
    CGoblin();
    virtual ~CGoblin();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnGoblinRefine(Answer::NetPacket* inPacket);
    int32_t OnGetShouHuAct(Answer::NetPacket* inPacket);
    int32_t OnShouHuRefine(Answer::NetPacket* inPacket);
    int32_t OnEquipPolish(Answer::NetPacket* inPacket);
    int32_t OnEquipPolishLock(Answer::NetPacket* inPacket);
    int32_t OnWingEquipPolish(Answer::NetPacket* inPacket);
    int32_t OnWingEquipPolishLock(Answer::NetPacket* inPacket);
    int32_t OnEquipPolishUp(Answer::NetPacket* inPacket);

private:
    int32_t m_nShouHuAct;
    int32_t m_nShouHuExp;
    int32_t m_nLastPolishTime;
};

#endif // _CGOBLIN_H_
