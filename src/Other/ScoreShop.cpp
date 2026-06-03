#include "common.h"
#include "Other/ScoreShop.h"

#include <cstring>

ScoreShop::ScoreShop()
    : CExtSystemBase()
{
}

ScoreShop::~ScoreShop()
{
}

void ScoreShop::OnCleanUp()
{
}

void ScoreShop::OnUpdate(int64_t curTick)
{
}

void ScoreShop::OnDaySwitch(int32_t nDiffDays)
{
    auto it = m_ItemLimit.begin();
    while (it != m_ItemLimit.end())
    {
        CfgData *pCfg = CfgData::instance();
        ScoreShopCfg *pShop = CfgData::GetScoreShopCfg(pCfg, it->first);
        if (pShop && pShop->DayLimitType == 1)
        {
            auto tmp = it;
            ++it;
            m_ItemLimit.erase(tmp);
        }
        else
        {
            ++it;
        }
    }
}

void ScoreShop::OnLoadFromDB(const PlayerDBData *dbData)
{
    m_ItemLimit = dbData->m_ScoreShopData.m_LiMitMap;
}

void ScoreShop::OnSaveToDB(PlayerDBData *dbData)
{
    dbData->m_ScoreShopData.m_LiMitMap = m_ItemLimit;
}

void ScoreShop::GetInterestsProtocol(ProcIdList *procList)
{
    procList->push_back(451);
    procList->push_back(453);
}

int32_t ScoreShop::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 2;

    if (nProcId == 451)
        return OnBuyItem(inPacket);
    if (nProcId != 453)
        return 2;

    SendLimitInfo(0);
    return 0;
}

int32_t ScoreShop::OnBuyItem(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int32_t nIndex = inPacket->readInt32();
    int32_t nCount = inPacket->readInt32();
    if (nCount <= 0)
        return 2;

    CfgData *pCfg = CfgData::instance();
    ScoreShopCfg *pShop = CfgData::GetScoreShopCfg(pCfg, nIndex);
    if (!pShop)
        return 2;

    CfgData *pCfg2 = CfgData::instance();
    int32_t nLayNum = CfgData::getOverlay(pCfg2, pShop->Item.itemId, pShop->Item.itemClass);
    if (nCount * pShop->Item.itemCount > nLayNum)
        return 2;

    CExtCharBag *pBag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::GetFreeSlotCount(pBag) <= 0)
        return 2;

    if (pShop->CostType < 0 || pShop->CostValue <= 0)
        return 2;

    if (pShop->LimitType > 0)
    {
        int32_t nLimit = GetLimitCount(nIndex);
        if (nCount + nLimit > pShop->LimitCount)
            return 2;
    }

    int32_t nCostValue = nCount * pShop->CostValue;
    if (Player::GetCurrency(m_pPlayer, (CURRENCY_TYPE)pShop->CostType) < nCostValue)
        return 2;

    if (!pShop->CostItems.empty())
    {
        ItemDataList costItems = pShop->CostItems;
        for (auto it = costItems.begin(); it != costItems.end(); ++it)
            it->m_nCount *= nCount;

        if (!CExtCharBag::RemoveItem(pBag, &costItems, ITEM_CHANGE_REASON::ICR_SCORE_SHOP_COST))
            return 2;
    }

    if (!Player::DecCurrency(m_pPlayer, (CURRENCY_TYPE)pShop->CostType, nCostValue, CURRENCY_CHANGE_REASON::SCR_BUY_ITEM, nIndex))
        return 2;

    MemChrBag item;
    memset(&item, 0, sizeof(item));
    item = pShop->Item;
    item.itemCount *= nCount;

    MemChrBagVector items;
    items.push_back(item);
    CExtCharBag::AddItem(pBag, &items, ITEM_CHANGE_REASON::ICR_SCORE_SHOP_GET);

    if (pShop->LimitType > 0)
    {
        AddLimitCount(nIndex, nCount);
        SendLimitInfo(nIndex);
    }

    if (pShop->IsRecord > 0)
        AddServerRecord(&item);

    int64_t nRet = nIndex;
    uint16_t nProc = inPacket->getProc();
    int16_t nGateIndex = Player::getGateIndex(m_pPlayer);
    int8_t nConnId = Player::getConnId(m_pPlayer);
    GameService *pGame = GameService::instance();
    GameService::replySuccess(pGame, nConnId, nGateIndex, nProc, nRet);
    return 0;
}

void ScoreShop::AddServerRecord(MemChrBag *Item)
{
    if (!m_pPlayer)
        return;

    GameService *pGame = GameService::instance();
    int8_t nConnId = Player::getConnId(m_pPlayer);
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_PROC, 0x4EBA);
    if (!packet)
        return;

    int16_t nGateIndex = Player::getGateIndex(m_pPlayer);
    packet->writeInt32(nGateIndex);
    CharId_t nCid = Player::getCid(m_pPlayer);
    packet->writeInt64(nCid);
    std::string val;
    Player::getName(m_pPlayer, &val);
    packet->writeUTF8(&val);
    packet->writeInt8(Item->itemClass);
    packet->writeInt32(Item->itemId);
    packet->writeInt32(Item->itemCount);
    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, Player::getConnId(m_pPlayer), packet);
}

void ScoreShop::SendLimitInfo(int32_t Index)
{
    GameService *pGame = GameService::instance();
    int8_t nConnId = Player::getConnId(m_pPlayer);
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x2CCD);
    if (!packet)
        return;

    if (Index <= 0)
    {
        packet->writeInt32((int32_t)m_ItemLimit.size());
        for (auto &pair : m_ItemLimit)
        {
            packet->writeInt32(pair.first);
            packet->writeInt32(pair.second);
        }
    }
    else
    {
        auto it = m_ItemLimit.find(Index);
        if (it != m_ItemLimit.end())
        {
            packet->writeInt32(1);
            packet->writeInt32(it->first);
            packet->writeInt32(it->second);
        }
        else
        {
            packet->writeInt32(1);
            packet->writeInt32(0);
            packet->writeInt32(0);
        }
    }

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    int16_t nGateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, Player::getConnId(m_pPlayer), nGateIndex, packet);
}

int32_t ScoreShop::GetLimitCount(int32_t index)
{
    auto it = m_ItemLimit.find(index);
    if (it != m_ItemLimit.end())
        return it->second;
    return 0;
}

void ScoreShop::AddLimitCount(int32_t index, int32_t count)
{
    auto it = m_ItemLimit.find(index);
    if (it != m_ItemLimit.end())
    {
        it->second += count;
    }
    else
    {
        m_ItemLimit[index] = count;
    }
}
