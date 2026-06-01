#ifndef _CBLACKMARKET_H_
#define _CBLACKMARKET_H_

#include "Character/CExtSystemBase.h"

class CBlackMarket : public CExtSystemBase
{
public:
    CBlackMarket();
    virtual ~CBlackMarket();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* const procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t onRequestMarketInfo(Answer::NetPacket* inPacket);
    int32_t onEnterMarket(Answer::NetPacket* inPacket);
    int32_t onBuyMarketGoods(Answer::NetPacket* inPacket);
    void sendMarketInfo();
    void SendMarketIcon();
    void GetMarketIconState(IconStateList* const IconList);
    void getMarketIconStu(ShowIcon* __return_ptr retstr);
    bool checkBuyRecord();
    void broadcastBuyItem(int32_t nBroadcast, const MemChrBag* const item);
};

#endif // _CBLACKMARKET_H_


