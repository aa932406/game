#ifndef _CURSE_H_
#define _CURSE_H_

#include "Character/CExtSystemBase.h"

class Curse : public CExtSystemBase
{
public:
    Curse();
    virtual ~Curse();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void AddCharAttr();
    int32_t OnCurseLevelUp(Answer::NetPacket* inPacket);
    int32_t DamageValue(int64_t MaxHp);
};

#endif // _CURSE_H_

