// CExtCharMysteryShop.h
#ifndef CEXTCHARMYSTERYSHOP_H
#define CEXTCHARMYSTERYSHOP_H

#include "CExtSystemBase.h"
#include <list>

struct MysteryShop
{
    int8_t nType;
    int32_t vGoods[4];
    int32_t nBuyFlag;
    int32_t nAutoRefreshTime;
};

class CExtCharMysteryShop : public CExtSystemBase
{
public:
    CExtCharMysteryShop();
    virtual ~CExtCharMysteryShop();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onAskShopInfo(Answer::NetPacket* inPacket);
    int32_t onBuyShopItem(Answer::NetPacket* inPacket);
    int32_t onRefreshShopItems(Answer::NetPacket* inPacket);
    
    void sendShopInfo(const MysteryShop* shop);
    void checkRefresh(MysteryShop* shop);
    void refreshShop(MysteryShop* shop, bool bAuto);
    int32_t getNextRefreshTime(int32_t nLastTime);
    int32_t refreshShopItem(int8_t nType, int32_t nLevel, void* excepts);
    
private:
    std::list<MysteryShop> m_lstShops;
};

#endif