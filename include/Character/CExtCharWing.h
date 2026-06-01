// CExtCharWing.h
#ifndef CEXTCHARWING_H
#define CEXTCHARWING_H

#include "CExtSystemBase.h"

class CExtCharWing : public CExtSystemBase
{
public:
    CExtCharWing();
    virtual ~CExtCharWing();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    int32_t onWingHuanHua(Answer::NetPacket* inPacket);
    int32_t onRequestInfo(Answer::NetPacket* inPacket);
    int32_t onWingUpLevel(Answer::NetPacket* inPacket);
    
    void ClearLuck();
    void SendWingInfo();
    void AddCharAttr();
    bool IsFunctionOpen();
    
private:
    int32_t m_Level;
    int32_t m_Luck;
    int64_t m_LaseUpdateTick;
    int32_t m_HuanHua;
};

#endif