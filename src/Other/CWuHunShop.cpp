#include "Other/CWuHunShop.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtCurrency.h"

CWuHunShop::CWuHunShop()
    : CExtSystemBase()
{
    m_WuHunShopMap.clear();
    m_RefreshTimes = 0;
    m_LastRefreshTime = 0;
}

CWuHunShop::~CWuHunShop()
{
}

void CWuHunShop::OnCleanUp()
{
    m_WuHunShopMap.clear();
    m_RefreshTimes = 0;
    m_LastRefreshTime = 0;
}

void CWuHunShop::OnUpdate(int64_t curTick)
{
}

void CWuHunShop::OnDaySwitch(int32_t nDiffDays)
{
    m_RefreshTimes = 5;
    m_LastRefreshTime = 0;
    sendShopRefreshTimes();
}

void CWuHunShop::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_WuHunShopMap = dbData->m_WuHunShopDBData.m_WuHunShopMap;
    m_RefreshTimes = dbData->m_WuHunShopDBData.m_RefreshTimes;
    m_LastRefreshTime = dbData->m_WuHunShopDBData.m_LastRefreshTime;
}

void CWuHunShop::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_WuHunShopDBData.m_WuHunShopMap = m_WuHunShopMap;
    dbData->m_WuHunShopDBData.m_RefreshTimes = m_RefreshTimes;
    dbData->m_WuHunShopDBData.m_LastRefreshTime = m_LastRefreshTime;
}

void CWuHunShop::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(290);
}

int32_t CWuHunShop::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    if (nProcId == 290)
        return OnWuHunShopAction(inPacket);
    return 10002;
}

int32_t CWuHunShop::OnWuHunShopAction(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    uint8_t nAct = inPacket->readUInt8();
    if (nAct == 1)
        return onAskShopInfo(inPacket);
    if (nAct == 2)
        return onBuyShopItem(inPacket);
    if (nAct == 3)
        return onRefreshShopItems(inPacket);
    return 10002;
}

void CWuHunShop::AddRefreshTimes(int32_t Times, bool IsNow)
{
    if (!m_pPlayer)
        return;
    if (m_RefreshTimes <= 4)
    {
        m_RefreshTimes += Times;
        if (IsNow)
            m_LastRefreshTime = Unit::getNow(m_pPlayer);
        if (m_RefreshTimes > 4)
        {
            m_RefreshTimes = 5;
            m_LastRefreshTime = 0;
        }
        sendShopRefreshTimes();
    }
    else
    {
        m_RefreshTimes = 5;
        m_LastRefreshTime = 0;
    }
}

void CWuHunShop::SubRefreshTimes()
{
    --m_RefreshTimes;
    if (!m_LastRefreshTime)
        m_LastRefreshTime = Unit::getNow(m_pPlayer);
    sendShopRefreshTimes();
}

int32_t CWuHunShop::onAskShopInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t ShopId = inPacket->readInt32();
    auto it = m_WuHunShopMap.find(ShopId);
    if (it == m_WuHunShopMap.end() && !RefreshShop(ShopId))
        return 10002;
    sendShopRefreshTimes();
    sendShopInfo(ShopId);
    return 0;
}

int32_t CWuHunShop::onBuyShopItem(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t ShopId = inPacket->readInt32();
    int8_t nIndex = inPacket->readInt8();
    auto it = m_WuHunShopMap.find(ShopId);
    if (it == m_WuHunShopMap.end())
        return 10002;
    if (static_cast<uint8_t>(nIndex) >= 4)
        return 10002;
    if (it->second.nState[nIndex] > 0)
        return 10002;
    int32_t nCfgId = it->second.nIndex[nIndex];
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    CfgWuHunShop* pCfg = CfgData::GetWuHunShopItem(cfg, nCfgId);
    if (!pCfg)
        return 10002;
    if (pCfg->Const > 0 && !Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_BLOOD, pCfg->Const, CURRENCY_CHANGE_REASON::GCR_WU_HUN_SHOP_BUY, 0))
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pCfg->Item, ITEM_CHANGE_REASON::ICR_WU_HUN_SHOP_BUY))
        return 10002;
    it->second.nState[nIndex] = 1;
    sendShopInfo(ShopId);
    return 0;
}

int32_t CWuHunShop::onRefreshShopItems(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t ShopId = inPacket->readInt32();
    auto it = m_WuHunShopMap.find(ShopId);
    if (it == m_WuHunShopMap.end())
        return 10002;
    if (m_RefreshTimes > 0)
    {
        SubRefreshTimes();
    }
    else if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, 100, CURRENCY_CHANGE_REASON::GCR_WU_HUN_SHOP_REFRESH, 0))
    {
        return 10002;
    }
    if (!RefreshShop(ShopId))
        return 10002;
    sendShopInfo(ShopId);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, ConnId, GateIndex, Proc, 0);
    return 0;
}

void CWuHunShop::sendShopInfo(int32_t ShopId)
{
    if (!m_pPlayer)
        return;
    auto it = m_WuHunShopMap.find(ShopId);
    if (it == m_WuHunShopMap.end())
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2E9Fu);
    if (!packet)
        return;
    packet->writeInt32(ShopId);
    packet->writeInt8(4);
    for (int i = 0; i <= 3; ++i)
    {
        packet->writeInt32(it->second.nIndex[i]);
        packet->writeInt8(it->second.nState[i]);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

void CWuHunShop::sendShopRefreshTimes()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2EA0u);
    if (!packet)
        return;
    packet->writeInt32(m_RefreshTimes);
    packet->writeInt32(m_LastRefreshTime);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

bool CWuHunShop::RefreshShop(int32_t ShopId)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    auto Items = CfgData::GetWuHunShopItemList(cfg);
    if (Items.size() <= 3)
        return false;
    int32_t MaxRand = 0;
    for (auto& item : Items)
        MaxRand += item.Rate;
    Answer::Random* rng = Answer::Singleton<Answer::Random>::instance();
    WuHunShop shop;
    memset(&shop, 0, sizeof(shop));
    for (int i = 0; i <= 3; ++i)
    {
        if (MaxRand <= 0)
            return false;
        int nRand = rng->generate(1, MaxRand);
        int32_t Accum = 0;
        for (auto& item : Items)
        {
            Accum += item.Rate;
            if (Accum >= nRand)
            {
                shop.nIndex[i] = item.Id;
                shop.nState[i] = 0;
                break;
            }
        }
    }
    m_WuHunShopMap[ShopId] = shop;
    return true;
}
