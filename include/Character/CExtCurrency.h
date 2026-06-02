// CExtCurrency.h
#ifndef CEXTCURRENCY_H
#define CEXTCURRENCY_H

#include "CExtSystemBase.h"
#include "Common/CommonTypes.h"

#define CURRENCY_MONEY 0
#define CURRENCY_BIND_MONEY 1
#define CURRENCY_GOLD 2
#define CURRENCY_BIND_GOLD 3
#define CURRENCY_HONOR 4
#define CURRENCY_VIGOUR 5
#define CURRENCY_CONTRIBUTION 6
#define CURRENCY_AC_SOCRE 7
#define CURRENCY_BOSS_SCORE 8
#define CURRENCY_CASH 9
#define CURRENCY_MAX 12

class CExtCurrency : public CExtSystemBase
{
public:
    CExtCurrency();
    virtual ~CExtCurrency();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    int32_t onSocialAddCurrency(Answer::NetPacket* inPacket);
    int32_t OnCurrencyDuiHuan(Answer::NetPacket* inPacket);
    int32_t syncGold(Answer::NetPacket* inPacket);
    
    int64_t GetMoneyBindAndNoBind();
    bool DecMoneyAndNoBind(int64_t nMoney, CURRENCY_CHANGE_REASON Reason, int32_t Id);
    void SendCurrencyInfo(bool bAll = false);
    void SynCurrency();
    
private:
    int64_t m_aCurrency[CURRENCY_MAX];
    bool m_aUpdateSign[CURRENCY_MAX];
    bool m_bNeedSync;
};

#endif