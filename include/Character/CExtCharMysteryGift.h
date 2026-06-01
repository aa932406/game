// CExtCharMysteryGift.h
#ifndef CEXTCHARMYSTERYGIFT_H
#define CEXTCHARMYSTERYGIFT_H

#include "CExtSystemBase.h"

class CExtCharMysteryGift : public CExtSystemBase
{
public:
    CExtCharMysteryGift();
    virtual ~CExtCharMysteryGift();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onGetGift(Answer::NetPacket* inPacket);
    int32_t onRequestInfo(Answer::NetPacket* inPacket);
    
    void SendMysteryGiftInfo(bool bCheckIndex);
    int32_t getLeftTime();
    int64_t getCondition();
    int8_t getState();
    void setState(int8_t nState);
    int32_t getIndex();
    void addIndex();
    void broadcastGetGift(int32_t nBroadId);
    
private:
    int32_t m_nIndex;
    int8_t m_nState;
};

#endif