// CExtCharCarrier.h
#ifndef CEXTCHARTCARRIER_H
#define CEXTCHARTCARRIER_H

#include "CExtSystemBase.h"

class CExtCharCarrier : public CExtSystemBase
{
public:
    CExtCharCarrier();
    virtual ~CExtCharCarrier();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onLeaveCarrier(Answer::NetPacket* inPacket);
    int32_t GetCarrierId();
    void EnterCarrier(int32_t nId);
    void LeaveCarrier();
    bool HaveSkill(int32_t nSkillId);
    bool IsInCarrier();
    
private:
    void* m_pCarrier;
};

#endif