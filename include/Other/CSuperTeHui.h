#ifndef _CSUPERTEHUI_H_
#define _CSUPERTEHUI_H_

#include "Character/CExtSystemBase.h"

class CSuperTeHui : public CExtSystemBase
{
public:
    CSuperTeHui();
    virtual ~CSuperTeHui();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnBuySuperTeHui(Answer::NetPacket* inPacket);
    void GetSuperTeHuiIcon(IconStateList* IconList);
    void SendSuperTeHuiIcon();
    void GetSuperTeHuiIcon(ShowIcon* __return_ptr retstr);
    bool IsAllGetTeHui();
    int32_t OnGetJewelPavilionItem(Answer::NetPacket* inPacket);
    void GetJewelPavilionIcon(IconStateList* IconList);
    void SendJewelPavilionIcon();
    void GetJewelPavilionIcon(ShowIcon* __return_ptr retstr);
    int32_t HaveFreeCount();
    bool IsOpenedJewelPavilion();
    bool IsAllGetPavilion();
    bool IsTimeEnd();
    void SetJewelPavilionOpen();
    int32_t GetJewelPavilionDay();
    int32_t OnBuyShengRenItem(Answer::NetPacket* inPacket);
    void GetShangRenIcon(IconStateList* IconList);
    void SendShangRenIcon();
    void GetShangRenIcon(ShowIcon* __return_ptr retstr);
    int32_t GetShangRenLeftTime();
    void SendLeftTime();
    bool IsSoldOut();
};

#endif // _CSUPERTEHUI_H_


