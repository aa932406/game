#ifndef _CWUHUNSHOP_H_
#define _CWUHUNSHOP_H_

#include "Character/CExtSystemBase.h"

class CWuHunShop : public CExtSystemBase
{
public:
    CWuHunShop();
    virtual ~CWuHunShop();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void AddRefreshTimes(int32_t Times, bool IsNow);
    void SubRefreshTimes();
    int32_t onAskShopInfo(Answer::NetPacket* inPacket);
    int32_t onBuyShopItem(Answer::NetPacket* inPacket);
    int32_t onRefreshShopItems(Answer::NetPacket* inPacket);
    void sendShopInfo(int32_t ShopId);
    void sendShopRefreshTimes();
    bool RefreshShop(int32_t ShopId);
};

#endif // _CWUHUNSHOP_H_

