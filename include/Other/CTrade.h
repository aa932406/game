#ifndef _CTRADE_H_
#define _CTRADE_H_

#include "Character/CExtSystemBase.h"
#include "Other/TradeData.h"

enum class OPERATE_VALUES
{
    OPERATE_AGREE_TRADE = 1,
    OPERATE_CANCEL,
    OPERATE_SET_LOCK,
    OPERATE_SET_SURED,
};

class CTrade : public CExtSystemBase
{
public:
    CTrade();
    virtual ~CTrade();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* const procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnAskTrade(Answer::NetPacket* inPacket);
    int32_t OnAgreeTrade(Answer::NetPacket* inPacket);
    int32_t OnAddItem(Answer::NetPacket* inPacket);
    int32_t OnRemoveItem(Answer::NetPacket* inPacket);
    int32_t OnAddMoney(Answer::NetPacket* inPacket);
    int32_t OnSubMoney(Answer::NetPacket* inPacket);
    int32_t OnLock(Answer::NetPacket* inPacket);
    int32_t OnSured(Answer::NetPacket* inPacket);
    int32_t OnCancel(Answer::NetPacket* inPacket);
    void SendAskTrade(Player* pTarget);
    void SendOperateToTarget(OPERATE_VALUES OperateType);
    void SendItemChange(int32_t TradeSlot);
    void SendTargetItemChange(int32_t TradeSlot);
    void SendTradeMoneyChange();
    void SendTargetTradeMoneychange();
    void RemoveItem(int32_t TradeSlot);
    int32_t GetDistance(Position Pos);
    void CheckDistance();
    bool CompareItem();
    bool RemoveBagItem();
    void AddBagItem(MemChrBagVector* const ItemVector);
    bool HaveSameBagSlot(int32_t BagSlot);

    void SetTargetChar(CharId_t id) { m_TargetId = id; }
    void SetTrading() { m_IsTrading = 1; }
    void SetLock() { m_IsLock = 1; }
    void SetSureTrad() { m_IsSureTrad = 1; }
    bool IsTrading() const { return m_IsTrading != 0; }
    bool IsLock() const { return m_IsLock != 0; }
    bool IsSureTrad() const { return m_IsSureTrad != 0; }
    void GetTradeItem(int32_t* Moeny, int32_t* Gold, MemChrBagVector* ItemVector);
    void SendTradeSucceed(int32_t* Moeny, int32_t* Gold, MemChrBagVector* ItemVector);

    TradeData m_TradeData[6];
    CharId_t m_TargetId;
    int8_t m_IsTrading;
    int8_t m_IsLock;
    int8_t m_IsSureTrad;
    int32_t m_TradMoney;
    int32_t m_TradGold;
};

#endif // _CTRADE_H_

