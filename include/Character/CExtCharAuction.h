// CExtCharAuction.h
#ifndef CEXTCHARAUCTION_H
#define CEXTCHARAUCTION_H

#include "CExtSystemBase.h"

struct MemChrBag
{
    int32_t itemId;
    int8_t itemClass;
    int32_t itemCount;
    int8_t bind;
    int32_t endTime;
    int64_t srcId;
};

class CExtCharAuction : public CExtSystemBase
{
public:
    CExtCharAuction();
    virtual ~CExtCharAuction();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onAuctionSell(Answer::NetPacket* inPacket);
    int32_t onAuctionBuy(Answer::NetPacket* inPacket);
    int32_t onOpenStall(Answer::NetPacket* inPacket);
    int32_t onCloseStall(Answer::NetPacket* inPacket);
    int32_t onAddEquip(Answer::NetPacket* inPacket);
    
    bool closeStall();
    bool IsInStall() const { return m_nStall != 0; }
    
private:
    void sendSocialBuy(int32_t nId);
    void sendSocialStall(bool bStall);
    
    int64_t m_nStall;
};

#endif