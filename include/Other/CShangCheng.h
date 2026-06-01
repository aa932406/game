#ifndef _CSHANGCHENG_H_
#define _CSHANGCHENG_H_

#include "Character/CExtSystemBase.h"

class CShangCheng : public CExtSystemBase
{
public:
    CShangCheng();
    virtual ~CShangCheng();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void SendResetLimitInfo();
    int32_t BuyShopItem(int32_t ShopId, int32_t ItemCount, bool AddBag);
    int32_t OnBuyShopItem(Answer::NetPacket* inPacket);
    int32_t OnAskLimitInfo(Answer::NetPacket* inPacket);
    void SendItemLimitChange(int32_t ShopId);
    int32_t OnBuyGameLimitItem(Answer::NetPacket* inPacket);
    void SendBuyItemSucceed(int32_t ShopId, int32_t AddCount);
    int32_t GetLimitCount(int32_t ShopId);
    void SendLimitInfo();
    void AddLimitCount(int32_t ShopId, int32_t AddCount);
    bool IsLimited(CfgGameShop* pShangCheng, int32_t ItemCount);
    void SendSocialBuyLimitItem(int32_t ShopId, int32_t Count);
    void SendSocialAskLimitInfo();
};

#endif // _CSHANGCHENG_H_

