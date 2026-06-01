// CExtCharWish.h
#ifndef CEXTCHARWISH_H
#define CEXTCHARWISH_H

#include "CExtSystemBase.h"
#include <list>
#include <string>

struct CharWishInfo
{
    int32_t nId;
    int32_t nItemId;
    int32_t nStartTime;
    std::string strInfo;
    int8_t nGetReward;
};

class CExtCharWish : public CExtSystemBase
{
public:
    CExtCharWish();
    virtual ~CExtCharWish();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    int32_t OnWish(Answer::NetPacket* inPacket);
    int32_t OnGetWishInfo(Answer::NetPacket* inPacket);
    int32_t OnGetWishReward(Answer::NetPacket* inPacket);
    
    void sendWishInfo();
    void SendWishIcon();
    void AppendWishInfo(Answer::NetPacket* packet);
    bool canGetReward();
    int32_t getLeftTime();
    
private:
    std::list<CharWishInfo> m_lstWishs;
    int32_t m_nNextId;
};

#endif