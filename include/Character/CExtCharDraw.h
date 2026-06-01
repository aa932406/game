// CExtCharDraw.h
#ifndef CEXTCHARDRAW_H
#define CEXTCHARDRAW_H

#include "CExtSystemBase.h"

class CExtCharDraw : public CExtSystemBase
{
public:
    CExtCharDraw();
    virtual ~CExtCharDraw();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    int32_t OnDraw(Answer::NetPacket* inPacket);
    int32_t GetItemRecord(int8_t nType);
    bool canDraw(int8_t nType);
    void addDrawTimes(int8_t nType, int32_t GetItemRecord);
    
private:
    int32_t m_nRecord;
};

#endif