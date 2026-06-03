#include "common.h"
#include "Other/CTrade.h"

#include "Character/CExtCharBag.h"
#include "Character/CChrDepot.h"
#include "Character/CFunctionOpen.h"
#include "Character/CVip.h"
#include "Other/TradeData.h"
#include "Other/CEquipManager.h"
#include "Common/GameService.h"
#include "Common/Player.h"
#include "Common/StaticObj.h"

CTrade::CTrade()
    : CExtSystemBase()
{
    OnCleanUp();
}

CTrade::~CTrade()
{
}

void CTrade::OnCleanUp()
{
    m_TargetId = 0;
    m_IsTrading = 0;
    m_IsLock = 0;
    m_IsSureTrad = 0;
    m_TradMoney = 0;
    m_TradGold = 0;
    for (int32_t i = 0; i < 6; ++i)
        TradeData::OnClanup(&m_TradeData[i]);
}

void CTrade::OnUpdate(int64_t curTick)
{
    CheckDistance();
}

void CTrade::OnDaySwitch(int32_t nDiffDays)
{
}

void CTrade::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CTrade::OnSaveToDB(PlayerDBData* dbData)
{
}

void CTrade::GetInterestsProtocol(ProcIdList* const procList)
{
    uint16_t procs[] = { 419, 420, 421, 422, 428, 424, 425, 426, 427 };
    for (uint32_t i = 0; i < sizeof(procs) / sizeof(procs[0]); ++i)
        procList->push_back(procs[i]);
}

int32_t CTrade::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (Player::GetFuLiLevel(m_pPlayer) == 1)
        return 2;
    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 9)
        return 2;
    if (!Unit::isAlive(m_pPlayer))
        return 2;

    switch (nProcId)
    {
    case 0x1A3: return OnAskTrade(inPacket);
    case 0x1A4: return OnAgreeTrade(inPacket);
    case 0x1A5: return OnAddItem(inPacket);
    case 0x1A6: return OnRemoveItem(inPacket);
    case 0x1A8: return OnSubMoney(inPacket);
    case 0x1A9: return OnLock(inPacket);
    case 0x1AA:
        if (OnSured(inPacket))
        {
            int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
            CharId_t TargetId = m_TargetId;
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            Player* pTarget = GameService::getPlayer(gs2, TargetId, RunnerId, 1);
            if (pTarget)
            {
                CTrade* PlayerTrade = Player::GetPlayerTrade(pTarget);
                PlayerTrade->OnCleanUp();
            }
            OnCleanUp();
        }
        return 0;
    case 0x1AB: return OnCancel(inPacket);
    case 0x1AC: return OnAddMoney(inPacket);
    default: return 0;
    }
}

int32_t CTrade::OnAskTrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (IsTrading())
        return 2;

    CharId_t TargetId = Answer::NetPacket::readInt64(inPacket);
    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!pTarget)
        return 2;
    if (Player::GetSysSettingInfo(pTarget, 4))
    {
        Player::TiShiInfo(m_pPlayer, 5, 0);
        return 2;
    }

    CChrDepot* CharDepot = Player::GetCharDepot(m_pPlayer);
    if (!CChrDepot::IsEnterSecondPasswordOperate(CharDepot))
    {
        Player::TiShiInfo(m_pPlayer, 29, 0);
        return 2;
    }

    CChrDepot* v6 = Player::GetCharDepot(pTarget);
    if (!CChrDepot::IsEnterSecondPasswordOperate(v6))
    {
        Player::TiShiInfo(m_pPlayer, 30, 0);
        return 2;
    }

    CFunctionOpen* FuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (!CFunctionOpen::IsOpened(FuncOpen, 103))
        return 2;

    CFunctionOpen* v8 = Player::GetPlayerFunctionOpen(pTarget);
    if (!CFunctionOpen::IsOpened(v8, 103))
    {
        Player::TiShiInfo(m_pPlayer, 7, 0);
        return 2;
    }

    int32_t MapId = StaticObj::getMapId(pTarget);
    if (MapId != StaticObj::getMapId(m_pPlayer))
        return 2;

    CTrade* PlayerTrade = Player::GetPlayerTrade(pTarget);
    if (CTrade::IsTrading(PlayerTrade))
    {
        uint16_t Proc = Answer::NetPacket::getProc(inPacket);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        return GameService::replyfailure(gs2, ConnId, GateIndex, Proc, 1, 0);
    }
    if (IsTrading())
        return 2;

    Position TarPos = StaticObj::getCurrentTile(pTarget);
    Position CurrentTile = StaticObj::getCurrentTile(m_pPlayer);
    int32_t Distance = Position::tileDistance(&CurrentTile, &TarPos);
    if (Distance > 20)
        return 2;

    SendAskTrade(pTarget);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs3 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs3, ConnId, GateIndex, Proc, 0);
    return 0;
}

int32_t CTrade::OnAgreeTrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (IsTrading())
        return 2;

    CharId_t TargetId = Answer::NetPacket::readInt64(inPacket);
    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!pTarget)
        return 2;

    int32_t MapId = StaticObj::getMapId(pTarget);
    if (MapId != StaticObj::getMapId(m_pPlayer))
        return 2;

    CTrade* PlayerTrade = Player::GetPlayerTrade(pTarget);
    if (PlayerTrade->IsTrading())
    {
        uint16_t Proc = Answer::NetPacket::getProc(inPacket);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        return GameService::replyfailure(gs2, ConnId, GateIndex, Proc, 1, 0);
    }

    Position TarPos = StaticObj::getCurrentTile(pTarget);
    Position CurrentTile = StaticObj::getCurrentTile(m_pPlayer);
    int32_t Distance = Position::tileDistance(&CurrentTile, &TarPos);
    if (Distance > 20)
        return 2;

    SetTargetChar(TargetId);
    SetTrading();
    CharId_t Cid = Player::getCid(m_pPlayer);
    PlayerTrade->SetTargetChar(Cid);
    PlayerTrade->SetTrading();
    SendOperateToTarget(OPERATE_VALUES::OPERATE_AGREE_TRADE);
    CharId_t MyCid = Player::getCid(m_pPlayer);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs3 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs3, ConnId, GateIndex, Proc, MyCid);
    return 0;
}

int32_t CTrade::OnAddItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (!IsTrading())
        return 2;
    if (IsLock() || IsSureTrad())
        return 2;

    int32_t BagSlot = Answer::NetPacket::readInt32(inPacket);
    int8_t TradeSlot = Answer::NetPacket::readInt8(inPacket);
    if (BagSlot < 0)
        return 2;
    if ((uint8_t)TradeSlot > 6u)
        return 2;
    if (HaveSameBagSlot(BagSlot))
        return 2;

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    MemChrBag BagItem = *CExtCharBag::GetSlotData(Bag, BagSlot);
    if (BagItem.itemId <= 0 || BagItem.itemCount <= 0)
        return 2;
    if (BagItem.bind == 1 || BagItem.bind == 2)
        return 2;
    if (m_TradeData[TradeSlot].CharBagData.itemId > 0 || m_TradeData[TradeSlot].CharBagData.itemCount > 0)
        return 2;

    TradeData TradItem;
    TradItem.BagSlot = BagSlot;
    TradItem.CharBagData = BagItem;
    m_TradeData[TradeSlot] = TradItem;
    SendItemChange(TradeSlot);
    SendTargetItemChange(TradeSlot);
    return 0;
}

int32_t CTrade::OnRemoveItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (!IsTrading())
        return 2;
    if (IsLock() || IsSureTrad())
        return 2;

    int8_t TradeSlot = Answer::NetPacket::readInt8(inPacket);
    if ((uint8_t)TradeSlot > 6u)
        return 2;
    if (m_TradeData[TradeSlot].CharBagData.itemId <= 0 || m_TradeData[TradeSlot].CharBagData.itemCount <= 0)
        return 2;

    RemoveItem(TradeSlot);
    SendItemChange(TradeSlot);
    SendTargetItemChange(TradeSlot);
    return 0;
}

int32_t CTrade::OnAddMoney(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (!IsTrading())
        return 2;
    if (IsLock() || IsSureTrad())
        return 2;

    int8_t AddMoneyType = Answer::NetPacket::readInt8(inPacket);
    int32_t AddMoneyCount = Answer::NetPacket::readInt32(inPacket);
    if ((uint8_t)AddMoneyType >= 2u)
        return 2;

    CVip* PlayerVip = Player::GetPlayerVip(m_pPlayer);
    if (!CVip::CanAuction(PlayerVip))
        return 2;

    if (AddMoneyType == 1 && Player::GetFuLiLevel(m_pPlayer) == 1)
    {
        int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
        CharId_t TargetId = m_TargetId;
        GameService* gs = Answer::Singleton<GameService>::instance();
        Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
        if (pTarget)
        {
            int64_t Proc = Answer::NetPacket::getProc(inPacket);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            int8_t ConnId = Player::getConnId(m_pPlayer);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::replySuccess(gs2, ConnId, GateIndex, 0x1AB, Proc);
            SendOperateToTarget(OPERATE_VALUES::OPERATE_CANCEL);
            OnCleanUp();
            CTrade* PlayerTrade = Player::GetPlayerTrade(pTarget);
            PlayerTrade->OnCleanUp();
        }
        return 2;
    }

    if (AddMoneyCount <= 0)
        return 2;
    if (Player::GetCurrency(m_pPlayer, (CURRENCY_TYPE)AddMoneyType) < AddMoneyCount)
        return 2;

    if (AddMoneyType)
        m_TradGold = AddMoneyCount;
    else
        m_TradMoney = AddMoneyCount;
    SendTradeMoneyChange();
    SendTargetTradeMoneychange();
    return 0;
}

int32_t CTrade::OnSubMoney(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (!IsTrading())
        return 2;
    if (!IsLock() && !IsSureTrad())
    {
        Answer::NetPacket::readInt8(inPacket);
        Answer::NetPacket::readInt32(inPacket);
    }
    return 2;
}

int32_t CTrade::OnLock(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 2;
    SetLock();
    SendOperateToTarget(OPERATE_VALUES::OPERATE_SET_LOCK);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, ConnId, GateIndex, Proc, 0);
    return 0;
}

int32_t CTrade::OnSured(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (!IsLock())
        return 2;

    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    CharId_t TargetId = m_TargetId;
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!pTarget)
        return 2;

    CChrDepot* CharDepot = Player::GetCharDepot(m_pPlayer);
    if (!CChrDepot::IsEnterSecondPasswordOperate(CharDepot))
    {
        Player::TiShiInfo(m_pPlayer, 29, 0);
        return 2;
    }

    CChrDepot* v7 = Player::GetCharDepot(pTarget);
    if (!CChrDepot::IsEnterSecondPasswordOperate(v7))
    {
        Player::TiShiInfo(m_pPlayer, 30, 0);
        return 2;
    }

    CTrade* PlayerTrade = Player::GetPlayerTrade(pTarget);
    if (!PlayerTrade->IsSureTrad())
    {
        SetSureTrad();
        SendOperateToTarget(OPERATE_VALUES::OPERATE_SET_SURED);
        uint16_t Proc = Answer::NetPacket::getProc(inPacket);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::replySuccess(gs2, ConnId, GateIndex, Proc, 0);
        return 0;
    }

    int32_t Money = 0, Gold = 0;
    int32_t TargetMoney = 0, TargetGold = 0;
    MemChrBagVector ItemVector, TargetItemVector;

    GetTradeItem(&Money, &Gold, &ItemVector);
    PlayerTrade->GetTradeItem(&TargetMoney, &TargetGold, &TargetItemVector);

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    int32_t BagFreeCount = CExtCharBag::GetFreeSlotCount(Bag);
    CExtCharBag* TargetBag = Player::GetBag(pTarget);
    int32_t TargetBagFreeCount = CExtCharBag::GetFreeSlotCount(TargetBag);

    if ((int32_t)TargetItemVector.size() > BagFreeCount)
    {
        uint16_t v13 = Answer::NetPacket::getProc(inPacket);
        int16_t v14 = Player::getGateIndex(pTarget);
        int8_t v15 = Player::getConnId(pTarget);
        GameService* v16 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v16, v15, v14, v13, 2, 0);
        uint16_t v17 = Answer::NetPacket::getProc(inPacket);
        int16_t v18 = Player::getGateIndex(m_pPlayer);
        int8_t v19 = Player::getConnId(m_pPlayer);
        GameService* v20 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v20, v19, v18, v17, 3, 0);
        return 2;
    }

    if ((int32_t)ItemVector.size() > TargetBagFreeCount)
    {
        uint16_t v22 = Answer::NetPacket::getProc(inPacket);
        int16_t v23 = Player::getGateIndex(pTarget);
        int8_t v24 = Player::getConnId(pTarget);
        GameService* v25 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v25, v24, v23, v22, 3, 0);
        uint16_t v26 = Answer::NetPacket::getProc(inPacket);
        int16_t v27 = Player::getGateIndex(m_pPlayer);
        int8_t v28 = Player::getConnId(m_pPlayer);
        GameService* v29 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v29, v28, v27, v26, 2, 0);
        return 2;
    }

    if (TargetMoney > Player::GetCurrency(pTarget, CURRENCY_TYPE::CURRENCY_MONEY))
    {
        uint16_t v31 = Answer::NetPacket::getProc(inPacket);
        int16_t v32 = Player::getGateIndex(pTarget);
        int8_t v33 = Player::getConnId(pTarget);
        GameService* v34 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v34, v33, v32, v31, 4, 0);
        uint16_t v35 = Answer::NetPacket::getProc(inPacket);
        int16_t v36 = Player::getGateIndex(m_pPlayer);
        int8_t v37 = Player::getConnId(m_pPlayer);
        GameService* v38 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v38, v37, v36, v35, 5, 0);
        return 2;
    }

    if (TargetGold > Player::GetCurrency(pTarget, CURRENCY_TYPE::CURRENCY_GOLD))
    {
        uint16_t v40 = Answer::NetPacket::getProc(inPacket);
        int16_t v41 = Player::getGateIndex(pTarget);
        int8_t v42 = Player::getConnId(pTarget);
        GameService* v43 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v43, v42, v41, v40, 6, 0);
        uint16_t v44 = Answer::NetPacket::getProc(inPacket);
        int16_t v45 = Player::getGateIndex(m_pPlayer);
        int8_t v46 = Player::getConnId(m_pPlayer);
        GameService* v47 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v47, v46, v45, v44, 7, 0);
        return 2;
    }

    if (Money > Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_MONEY))
    {
        uint16_t v49 = Answer::NetPacket::getProc(inPacket);
        int16_t v50 = Player::getGateIndex(pTarget);
        int8_t v51 = Player::getConnId(pTarget);
        GameService* v52 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v52, v51, v50, v49, 5, 0);
        uint16_t v53 = Answer::NetPacket::getProc(inPacket);
        int16_t v54 = Player::getGateIndex(m_pPlayer);
        int8_t v55 = Player::getConnId(m_pPlayer);
        GameService* v56 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v56, v55, v54, v53, 4, 0);
        return 2;
    }

    if (Gold > Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD))
    {
        uint16_t v58 = Answer::NetPacket::getProc(inPacket);
        int16_t v59 = Player::getGateIndex(pTarget);
        int8_t v60 = Player::getConnId(pTarget);
        GameService* v61 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v61, v60, v59, v58, 7, 0);
        uint16_t v62 = Answer::NetPacket::getProc(inPacket);
        int16_t v63 = Player::getGateIndex(m_pPlayer);
        int8_t v64 = Player::getConnId(m_pPlayer);
        GameService* v65 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v65, v64, v63, v62, 6, 0);
        return 2;
    }

    if (!CompareItem())
    {
        uint16_t Proc = Answer::NetPacket::getProc(inPacket);
        int16_t GateIndex = Player::getGateIndex(pTarget);
        int8_t ConnId = Player::getConnId(pTarget);
        GameService* v69 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v69, ConnId, GateIndex, Proc, 9, 0);
        uint16_t v70 = Answer::NetPacket::getProc(inPacket);
        int16_t v71 = Player::getGateIndex(m_pPlayer);
        int8_t v72 = Player::getConnId(m_pPlayer);
        GameService* v73 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v73, v72, v71, v70, 8, 0);
        return 2;
    }

    if (!PlayerTrade->CompareItem())
    {
        uint16_t v75 = Answer::NetPacket::getProc(inPacket);
        int16_t v76 = Player::getGateIndex(pTarget);
        int8_t v77 = Player::getConnId(pTarget);
        GameService* v78 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v78, v77, v76, v75, 8, 0);
        uint16_t v79 = Answer::NetPacket::getProc(inPacket);
        int16_t v80 = Player::getGateIndex(m_pPlayer);
        int8_t v81 = Player::getConnId(m_pPlayer);
        GameService* v82 = Answer::Singleton<GameService>::instance();
        GameService::replyfailure(v82, v81, v80, v79, 9, 0);
        return 2;
    }

    for (MemChrBagVector::iterator iter = TargetItemVector.begin(); iter != TargetItemVector.end(); ++iter)
    {
        if (iter->itemClass == 2)
        {
            MemEquip equip;
            CEquipManager* cem = Answer::Singleton<CEquipManager>::instance();
            CEquipManager::GetMemEquip(&equip, cem, iter->srcId);
            if (equip.base == iter->itemId)
            {
                equip.owner = Player::getCid(m_pPlayer);
                int8_t myConn = Player::getConnId(m_pPlayer);
                CEquipManager* cem2 = Answer::Singleton<CEquipManager>::instance();
                CEquipManager::UpdateMemEquip(cem2, myConn, &equip, 8);
                int8_t targetConn = Player::getConnId(pTarget);
                if (myConn != targetConn)
                {
                    CEquipManager* cem3 = Answer::Singleton<CEquipManager>::instance();
                    CEquipManager::UpdateMemEquip(cem3, targetConn, &equip, 8);
                }
            }
        }
    }

    for (MemChrBagVector::iterator iter = ItemVector.begin(); iter != ItemVector.end(); ++iter)
    {
        if (iter->itemClass == 2)
        {
            MemEquip equip;
            CEquipManager* cem = Answer::Singleton<CEquipManager>::instance();
            CEquipManager::GetMemEquip(&equip, cem, iter->srcId);
            if (equip.base == iter->itemId)
            {
                equip.owner = Player::getCid(pTarget);
                int8_t myConn = Player::getConnId(m_pPlayer);
                CEquipManager* cem2 = Answer::Singleton<CEquipManager>::instance();
                CEquipManager::UpdateMemEquip(cem2, myConn, &equip, 8);
                int8_t targetConn = Player::getConnId(pTarget);
                if (myConn != targetConn)
                {
                    CEquipManager* cem3 = Answer::Singleton<CEquipManager>::instance();
                    CEquipManager::UpdateMemEquip(cem3, targetConn, &equip, 8);
                }
            }
        }
    }

    RemoveBagItem();
    PlayerTrade->RemoveBagItem();
    AddBagItem(&TargetItemVector);
    PlayerTrade->AddBagItem(&ItemVector);

    if (Money > 0)
    {
        CharId_t targetCid = Player::getCid(pTarget);
        Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_MONEY, Money, CURRENCY_CHANGE_REASON::MCR_TRADE_CONST_MONEY, targetCid);
        CVip* vip = Player::GetPlayerVip(pTarget);
        double rate = CVip::GetVipLevel(vip) <= 3 ? 0.8 : 0.98;
        Money = (int)((double)Money * rate);
        CharId_t myCid = Player::getCid(m_pPlayer);
        Player::AddCurrency(pTarget, CURRENCY_TYPE::CURRENCY_MONEY, Money, CURRENCY_CHANGE_REASON::MCR_TRADE_GET_MONEY, myCid);
    }

    if (Gold > 0)
    {
        CharId_t targetCid = Player::getCid(pTarget);
        Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, Gold, CURRENCY_CHANGE_REASON::GCR_TRADE_CONST_GOLD, targetCid);
        CVip* vip = Player::GetPlayerVip(pTarget);
        double rate = CVip::GetVipLevel(vip) <= 3 ? 0.8 : 0.98;
        Gold = (int)((double)Gold * rate);
        CharId_t myCid = Player::getCid(m_pPlayer);
        Player::AddCurrency(pTarget, CURRENCY_TYPE::CURRENCY_GOLD, Gold, CURRENCY_CHANGE_REASON::GCR_TRADE_GET_GOLD, myCid);
    }

    if (TargetMoney > 0)
    {
        CharId_t myCid = Player::getCid(m_pPlayer);
        Player::DecCurrency(pTarget, CURRENCY_TYPE::CURRENCY_MONEY, TargetMoney, CURRENCY_CHANGE_REASON::MCR_TRADE_CONST_MONEY, myCid);
        CVip* vip = Player::GetPlayerVip(m_pPlayer);
        double rate = CVip::GetVipLevel(vip) <= 3 ? 0.8 : 0.98;
        TargetMoney = (int)((double)TargetMoney * rate);
        CharId_t targetCid = Player::getCid(pTarget);
        Player::AddCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_MONEY, TargetMoney, CURRENCY_CHANGE_REASON::MCR_TRADE_GET_MONEY, targetCid);
    }

    if (TargetGold > 0)
    {
        CharId_t myCid = Player::getCid(m_pPlayer);
        Player::DecCurrency(pTarget, CURRENCY_TYPE::CURRENCY_GOLD, TargetGold, CURRENCY_CHANGE_REASON::GCR_TRADE_CONST_GOLD, myCid);
        CVip* vip = Player::GetPlayerVip(m_pPlayer);
        double rate = CVip::GetVipLevel(vip) <= 3 ? 0.8 : 0.98;
        TargetGold = (int)((double)TargetGold * rate);
        CharId_t targetCid = Player::getCid(pTarget);
        Player::AddCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, TargetGold, CURRENCY_CHANGE_REASON::GCR_TRADE_GET_GOLD, targetCid);
    }

    SendTradeSucceed(&TargetMoney, &TargetGold, &TargetItemVector);
    PlayerTrade->SendTradeSucceed(&Money, &Gold, &ItemVector);
    OnCleanUp();
    PlayerTrade->OnCleanUp();
    return 0;
}

int32_t CTrade::OnCancel(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (!IsTrading())
        return 2;

    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    CharId_t TargetId = m_TargetId;
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!pTarget)
        return 2;

    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs2, ConnId, GateIndex, Proc, 0);
    SendOperateToTarget(OPERATE_VALUES::OPERATE_CANCEL);
    OnCleanUp();
    CTrade* PlayerTrade = Player::GetPlayerTrade(pTarget);
    PlayerTrade->OnCleanUp();
    return 0;
}

void CTrade::SendAskTrade(Player* pTarget)
{
    if (!pTarget || !m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(pTarget);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CA3);
    if (!packet)
        return;

    CharId_t Cid = Player::getCid(m_pPlayer);
    Answer::NetPacket::writeInt64(packet, Cid);
    int32_t level = *((int32_t (__fastcall**)(Player*))m_pPlayer->_vptr_Entity + 9))(m_pPlayer);
    Answer::NetPacket::writeInt32(packet, level);

    std::string val;
    Player::getName(m_pPlayer, &val);
    Answer::NetPacket::writeUTF8(packet, &val);

    int32_t avatar = *((int32_t (__fastcall**)(Player*))m_pPlayer->_vptr_Entity + 10))(m_pPlayer);
    Answer::NetPacket::writeInt32(packet, avatar);
    Sex_t Sex = Player::getSex(m_pPlayer);
    Answer::NetPacket::writeInt8(packet, Sex);
    Job_t Job = Player::getJob(m_pPlayer);
    Answer::NetPacket::writeInt8(packet, Job);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(pTarget);
    int8_t connId = Player::getConnId(pTarget);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId, GateIndex, packet);
}

void CTrade::SendOperateToTarget(OPERATE_VALUES OperateType)
{
    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    CharId_t TargetId = m_TargetId;
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!pTarget || !m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(pTarget);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CA9);
    if (!packet)
        return;

    Answer::NetPacket::writeInt8(packet, (int8_t)OperateType);
    if (OperateType == OPERATE_VALUES::OPERATE_AGREE_TRADE)
    {
        CharId_t Cid = Player::getCid(m_pPlayer);
        Answer::NetPacket::writeInt64(packet, Cid);
    }

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(pTarget);
    int8_t connId = Player::getConnId(pTarget);
    GameService* gs3 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs3, connId, GateIndex, packet);
}

void CTrade::SendItemChange(int32_t TradeSlot)
{
    if ((uint32_t)TradeSlot > 6 || !m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CA4);
    if (!packet)
        return;

    if (m_TradeData[TradeSlot].CharBagData.itemId < 0 || m_TradeData[TradeSlot].CharBagData.itemCount <= 0)
        m_TradeData[0].BagSlot = -1;

    Answer::NetPacket::writeInt8(packet, TradeSlot);
    Answer::NetPacket::writeInt32(packet, m_TradeData[TradeSlot].BagSlot);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId, GateIndex, packet);
}

void CTrade::SendTargetItemChange(int32_t TradeSlot)
{
    if ((uint32_t)TradeSlot > 6 || !m_pPlayer)
        return;

    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    CharId_t TargetId = m_TargetId;
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!pTarget)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CA5);
    if (!packet)
        return;

    if (m_TradeData[TradeSlot].CharBagData.itemId < 0 || m_TradeData[TradeSlot].CharBagData.itemCount <= 0)
        m_TradeData[0].BagSlot = -1;

    if (m_TradeData[TradeSlot].CharBagData.itemClass == 2)
    {
        EquipId_t srcId = m_TradeData[TradeSlot].CharBagData.srcId;
        MemEquip equip;
        CEquipManager* cem = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::GetMemEquip(&equip, cem, srcId);
        if (equip.base == m_TradeData[TradeSlot].CharBagData.itemId)
        {
            CEquipManager* cem2 = Answer::Singleton<CEquipManager>::instance();
            CEquipManager::SendPlayerEquipInfo(cem2, pTarget, &equip);
        }
    }

    Answer::NetPacket::writeInt8(packet, TradeSlot);
    Answer::NetPacket::writeInt32(packet, m_TradeData[TradeSlot].CharBagData.itemId);
    Answer::NetPacket::writeInt8(packet, m_TradeData[TradeSlot].CharBagData.itemClass);
    Answer::NetPacket::writeInt32(packet, m_TradeData[TradeSlot].CharBagData.itemCount);
    Answer::NetPacket::writeInt8(packet, m_TradeData[TradeSlot].CharBagData.bind);
    Answer::NetPacket::writeInt32(packet, m_TradeData[TradeSlot].CharBagData.endTime);
    Answer::NetPacket::writeInt64(packet, m_TradeData[TradeSlot].CharBagData.srcId);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(pTarget);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs3 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs3, connId, GateIndex, packet);
}

void CTrade::SendTradeMoneyChange()
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CA6);
    if (!packet)
        return;

    Answer::NetPacket::writeInt32(packet, m_TradMoney);
    Answer::NetPacket::writeInt32(packet, m_TradGold);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId, GateIndex, packet);
}

void CTrade::SendTargetTradeMoneychange()
{
    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    CharId_t TargetId = m_TargetId;
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!m_pPlayer || !pTarget)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CA7);
    if (!packet)
        return;

    Answer::NetPacket::writeInt32(packet, m_TradMoney);
    Answer::NetPacket::writeInt32(packet, m_TradGold);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(pTarget);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs3 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs3, connId, GateIndex, packet);
}

void CTrade::RemoveItem(int32_t TradeSlot)
{
    if ((uint32_t)TradeSlot <= 6)
    {
        TradeData TradeItem;
        memset(&TradeItem, 0, sizeof(TradeItem));
        TradeItem.BagSlot = -1;
        m_TradeData[TradeSlot] = TradeItem;
    }
}

int32_t CTrade::GetDistance(Position Pos)
{
    Position CurrentTile = StaticObj::getCurrentTile(m_pPlayer);
    return Position::tileDistance(&CurrentTile, &Pos);
}

void CTrade::CheckDistance()
{
    if (!IsTrading())
        return;

    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);
    CharId_t TargetId = m_TargetId;
    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (m_pPlayer && pTarget)
    {
        int32_t MapId = StaticObj::getMapId(pTarget);
        if (MapId != StaticObj::getMapId(m_pPlayer) || GetDistance(StaticObj::getCurrentTile(pTarget)) > 20)
        {
            OnCleanUp();
            CTrade* PlayerTrade = Player::GetPlayerTrade(pTarget);
            PlayerTrade->OnCleanUp();
        }
    }
    else
    {
        OnCleanUp();
    }
}

bool CTrade::CompareItem()
{
    if (!m_pPlayer)
        return false;
    for (int32_t i = 0; i <= 5; ++i)
    {
        if (m_TradeData[i].CharBagData.itemId > 0 && m_TradeData[i].CharBagData.itemCount > 0)
        {
            int32_t BagSlot = m_TradeData[i].BagSlot;
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            const MemChrBag* SlotData = CExtCharBag::GetSlotData(Bag, BagSlot);
            if (SlotData->bind != m_TradeData[i].CharBagData.bind
                || SlotData->endTime != m_TradeData[i].CharBagData.endTime
                || SlotData->itemClass != m_TradeData[i].CharBagData.itemClass
                || SlotData->itemId != m_TradeData[i].CharBagData.itemId
                || SlotData->itemCount != m_TradeData[i].CharBagData.itemCount
                || SlotData->srcId != m_TradeData[i].CharBagData.srcId)
            {
                return false;
            }
        }
    }
    return true;
}

bool CTrade::RemoveBagItem()
{
    if (!m_pPlayer)
        return false;

    for (int32_t i = 0; i <= 5; ++i)
    {
        if (m_TradeData[i].BagSlot >= 0
            && m_TradeData[i].CharBagData.itemId > 0
            && m_TradeData[i].CharBagData.itemCount > 0)
        {
            MemChrBag slotData;
            memset(&slotData, 0, sizeof(slotData));
            int32_t BagSlot = m_TradeData[i].BagSlot;
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            CExtCharBag::SetSlotData(Bag, BagSlot, &slotData, ITEM_CHANGE_REASON::ICR_TRADE, 0);
        }
    }
    return true;
}

void CTrade::AddBagItem(MemChrBagVector* const ItemVector)
{
    if (!m_pPlayer || ItemVector->empty())
        return;

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    CExtCharBag::AddItem(Bag, ItemVector, ITEM_CHANGE_REASON::ICR_TRADE);
}

bool CTrade::HaveSameBagSlot(int32_t BagSlot)
{
    for (int32_t i = 0; i <= 5; ++i)
    {
        if (m_TradeData[i].BagSlot >= 0 && m_TradeData[i].BagSlot == BagSlot)
            return true;
    }
    return false;
}

void CTrade::GetTradeItem(int32_t* Moeny, int32_t* Gold, MemChrBagVector* ItemVector)
{
    *Moeny = m_TradMoney;
    *Gold = m_TradGold;
    for (int32_t i = 0; i <= 5; ++i)
    {
        if (m_TradeData[i].BagSlot >= 0
            && m_TradeData[i].CharBagData.itemId > 0
            && m_TradeData[i].CharBagData.itemCount > 0)
        {
            ItemVector->push_back(m_TradeData[i].CharBagData);
        }
    }
}

void CTrade::SendTradeSucceed(int32_t* Moeny, int32_t* Gold, MemChrBagVector* ItemVector)
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CA8);
    if (!packet)
        return;

    Answer::NetPacket::writeInt32(packet, *Moeny);
    Answer::NetPacket::writeInt32(packet, *Gold);
    int8_t nSize = (int8_t)ItemVector->size();
    Answer::NetPacket::writeInt8(packet, nSize);
    for (MemChrBagVector::iterator it = ItemVector->begin(); it != ItemVector->end(); ++it)
    {
        Answer::NetPacket::writeInt32(packet, it->itemId);
        Answer::NetPacket::writeInt8(packet, it->itemClass);
        Answer::NetPacket::writeInt32(packet, it->itemCount);
        Answer::NetPacket::writeInt8(packet, it->bind);
        Answer::NetPacket::writeInt32(packet, it->endTime);
        Answer::NetPacket::writeInt64(packet, it->srcId);
    }

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId, GateIndex, packet);
}
