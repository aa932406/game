// CExtCharExchange.h
#ifndef CEXTCHAREXCHANGE_H
#define CEXTCHAREXCHANGE_H

#include "CExtSystemBase.h"
#include <list>

struct ExchangeRecord
{
    int8_t nType;
    int16_t nIndex;
    int32_t nCount;
};

class CExtCharExchange : public CExtSystemBase
{
public:
    CExtCharExchange();
    virtual ~CExtCharExchange();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onAskExchangeInfo(Answer::NetPacket* inPacket);
    int32_t onExchange(Answer::NetPacket* inPacket);
    
    void sendExchangeInfo(int8_t nType);
    int32_t getExchangeRecord(int8_t nType, int16_t nIndex);
    void addExchangeRecord(int8_t nType, int16_t nIndex, int32_t nCount);
    
private:
    std::list<ExchangeRecord> m_lstExchangeRecords;
};

#endif