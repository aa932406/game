#include "Other/CShangCheng.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "DBService.h"
#include "Other/CFunctionOpen.h"

CShangCheng::CShangCheng()
    : CExtSystemBase()
{
    m_ShopItem.clear();
    m_broadcastBuyItemList.clear();
}

CShangCheng::~CShangCheng()
{
}

void CShangCheng::OnCleanUp()
{
}

void CShangCheng::OnUpdate(int64_t curTick)
{
}

void CShangCheng::OnDaySwitch(int32_t nDiffDays)
{
}

void CShangCheng::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_ShopItem = dbData->m_ShangChengDBData.m_ShopItem;
}

void CShangCheng::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_ShangChengDBData.m_ShopItem = m_ShopItem;
}

void CShangCheng::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(404);
    procList->push_back(413);
    procList->push_back(415);
    procList->push_back(416);
    procList->push_back(417);
    procList->push_back(418);
    procList->push_back(420);
    procList->push_back(421);
    procList->push_back(422);
    procList->push_back(423);
    procList->push_back(20202);
    procList->push_back(20203);
    procList->push_back(20205);
}

int32_t CShangCheng::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId <= 418)
    {
        if (nProcId > 416)
        {
            if (nProcId == 417)
                return OnGetShangChengData(inPacket);
            if (nProcId == 418)
                return OnBroadcastBuyShopItem(inPacket);
        }
        else
        {
            if (nProcId == 404)
                return OnGetDailyData(inPacket);
            if (nProcId == 413)
                return OnBuyShopItem(inPacket);
            if (nProcId == 415)
                return OnBuyShopItem(inPacket);
            if (nProcId == 416)
                return OnBuyShopItem(inPacket);
        }
    }
    else
    {
        if (nProcId > 422)
        {
            if (nProcId == 420)
                return CShangCheng::OnGetBaoKuData(inPacket);
            if (nProcId == 421)
                return OnPickUpAllItems(inPacket);
            if (nProcId == 422)
                return OnPickUpOneItem(inPacket);
            if (nProcId == 423)
                return CShangCheng::OnGetBaoKuLog(inPacket);
        }
        else
        {
            if (nProcId == 20202)
                return OnTestShop(inPacket);
            if (nProcId == 20203)
                return OnTestShop(inPacket);
            if (nProcId == 20205)
                return OnS2CGMTool(inPacket);
        }
    }
    return 2;
}

int32_t CShangCheng::OnGetDailyData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, Proc + 24576);
    if (!packet)
        return 2;
    int32_t nCurDay = Unit::getDayCount(m_pPlayer);
    packet->writeInt32(nCurDay);
    int32_t nSize = m_ShopItem.size();
    packet->writeInt32(nSize);
    for (auto it = m_ShopItem.begin(); it != m_ShopItem.end(); ++it)
    {
        packet->writeInt32(it->first);
        packet->writeInt32(it->second);
    }
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CShangCheng::OnBuyShopItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int32_t ShopId = inPacket->readInt32();
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgShopTable* pShopTable = CfgData::GetShopTable(v4);
    const CfgShop* pCfgShop = CfgShopTable::GetRow(pShopTable, ShopId);
    if (!pCfgShop)
        return 2;
    int32_t nItemId = pCfgShop->m_ItemId;
    int32_t nItemCount = pCfgShop->m_ItemCount;
    int32_t nItemClass = pCfgShop->m_ItemClass;
    int32_t nItemBind = pCfgShop->m_ItemBind;
    int32_t nPriceType = pCfgShop->m_PriceType;
    int32_t nPrice = pCfgShop->m_Price;
    int32_t nShopType = pCfgShop->m_ShopType;
    int32_t nItemType = pCfgShop->m_ItemType;
    int32_t nLevelLimit = pCfgShop->m_LevelLimit;
    int32_t nVIPLimit = pCfgShop->m_VIPLevel;
    int32_t nCount = pCfgShop->m_Count;
    int32_t nExtraInt = 0;
    int32_t nNeedQQVip = 0;
    int32_t nLimitShopDay = 0;
    int32_t nIsBindItem = 0;
    int32_t nBuffId = 0;
    int32_t nBuffTime = 0;
    if (nCount == 0)
        nCount = -1;
    if (nItemClass == 4)
    {
        const CfgExtItemTable* pExtItemTable = CfgData::GetExtItemTable(v4);
        const CfgExtItem* pExtItem = CfgExtItemTable::GetRow(pExtItemTable, nItemId);
        if (pExtItem)
        {
            nBuffId = pExtItem->m_BuffId;
            nBuffTime = pExtItem->m_BuffTime;
        }
    }
    CFunctionOpen* pFuncOpen = Answer::Singleton<CFunctionOpen>::instance();
    if (!CFunctionOpen::IsOpenByPlayer(pFuncOpen, m_pPlayer, nItemId))
        return 2;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (nCount > 0)
    {
        auto it = m_ShopItem.find(ShopId);
        if (it != m_ShopItem.end() && it->second >= nCount)
            return 2;
    }
    if (nLevelLimit > 0 && Player::getLevel(m_pPlayer) < nLevelLimit)
        return 2;
    int32_t nFashionLvl = Player::getFashionLvl(m_pPlayer);
    if (nFashionLvl < nVIPLimit)
        return 2;
    if (pCfgShop->m_ProfLimit > 0 && !Player::hasProf(m_pPlayer, pCfgShop->m_ProfLimit))
        return 2;
    if (nPriceType == 1)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 2)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 3)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 4)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 5)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 6)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 7)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 8)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 9)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 10)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 11)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 12)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 13)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    else if (nPriceType == 16)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM, ShopId))
            return 2;
    }
    int32_t nNotifyType = pCfgShop->m_NotifyType;
    int32_t nBroadcast = pCfgShop->m_Broadcast;
    int32_t nSuccessRate = pCfgShop->m_SuccessRate;
    if (nSuccessRate > 0)
    {
        int32_t nRand = Unit::getRandom(m_pPlayer, 10000);
        if (nRand > nSuccessRate)
            return 2;
    }
    MemChrBagVector ItemList;
    ItemList.reserve(1);
    MemChrBag item;
    memset(&item, 0, sizeof(item));
    item.itemClass = nItemClass;
    item.itemId = nItemId;
    item.itemCount = nItemCount;
    item.bind = nItemBind;
    if (!CExtCharBag::AddItemsAndMingGe(pBag, &ItemList, ITEM_CHANGE_REASON::ICR_BUY_ITEM))
    {
        if (nPriceType == 1)
        {
            Player::AddCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_BUY_ITEM_BACK, 0);
        }
        return 2;
    }
    int8_t nIsBind = 0;
    if (nPriceType == 1)
    {
        nIsBind = 0;
    }
    else if (nPriceType == 2)
    {
        nIsBind = 1;
    }
    if (nIsBindItem == 1)
        nIsBind = 1;
    if (nCount > 0)
    {
        int32_t& nBuyCount = m_ShopItem[ShopId];
        ++nBuyCount;
    }
    if (nNotifyType == 1)
    {
        int32_t nShowType = 1;
        if (nShopType == 4 || nShopType == 2)
            nShowType = 2;
        else if (nShopType == 1)
            nShowType = 3;
        MString strName;
        Player::getPlayerName(m_pPlayer, &strName);
        MemChrBagVector vItem;
        vItem.reserve(1);
        vItem.push_back(item);
        GameService* v22 = Answer::Singleton<GameService>::instance();
        GameService::SendItemFlowTip(v22, &strName, &vItem, nShowType);
    }
    if (nBroadcast > 0)
    {
        BroadcastBuyShopItem(ShopId, 0);
    }
    if (nItemClass == 11 && nBuffId > 0 && nBuffTime > 0)
    {
        CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
        CExtBuffManager::AddBuff_NoUseMessage(pBuffMgr, nBuffId, nBuffTime, 0, 0);
    }
    uint16_t v26 = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v28 = Player::getConnId(m_pPlayer);
    GameService* v29 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v29, v28, GateIndex, v26, 0);
    return 0;
}

int32_t CShangCheng::OnGetShangChengData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2C97);
    if (!packet)
        return 2;
    uint16_t nNormalShop = 0;
    uint16_t nDailyShop = 0;
    uint16_t nMianFeiShop = 0;
    uint16_t nHuoDongShop = 0;
    uint16_t nWanFaShop = 0;
    uint16_t nGongGaoShop = 0;
    uint16_t nPeiZhiShop = 0;
    uint16_t nVIPShop = 0;
    uint16_t nCFShop = 0;
    uint16_t nDaLuanShop = 0;
    uint16_t nHuiZhanShop = 0;
    uint16_t nJingJiShop = 0;
    uint16_t nNewServerShop = 0;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgShopTable* pShopTable = CfgData::GetShopTable(v4);
    int32_t nCount = 0;
    uint32_t nCountPos = Answer::NetPacket::getWOffset(packet);
    packet->writeInt32(0);
    CfgShopTable::Iterator* it = CfgShopTable::GetIterator(pShopTable);
    if (it)
    {
        do
        {
            const CfgShop* pShop = CfgShopTable::Iterator::getData(it);
            if (pShop && pShop->m_ShopType > 0)
            {
                if (pShop->m_ShopType == 1)
                {
                    ++nNormalShop;
                    if (nNormalShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 2)
                {
                    ++nDailyShop;
                    if (nDailyShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 3)
                {
                    ++nMianFeiShop;
                    if (nMianFeiShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 4)
                {
                    ++nHuoDongShop;
                    if (nHuoDongShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 5)
                {
                    ++nWanFaShop;
                    if (nWanFaShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 6)
                {
                    ++nGongGaoShop;
                    if (nGongGaoShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 7)
                {
                    ++nPeiZhiShop;
                    if (nPeiZhiShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 8)
                {
                    ++nVIPShop;
                    if (nVIPShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 9)
                {
                    ++nCFShop;
                    if (nCFShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 10)
                {
                    ++nDaLuanShop;
                    if (nDaLuanShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 11)
                {
                    ++nHuiZhanShop;
                    if (nHuiZhanShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 12)
                {
                    ++nJingJiShop;
                    if (nJingJiShop > 200)
                        continue;
                }
                else if (pShop->m_ShopType == 13)
                {
                    ++nNewServerShop;
                    if (nNewServerShop > 200)
                        continue;
                }
                int32_t nLevel = Player::getLevel(m_pPlayer);
                if (pShop->m_BuyMinLvl <= nLevel)
                {
                    int32_t nVipLevel = Player::getFashionLvl(m_pPlayer);
                    if (pShop->m_VIPLevel <= nVipLevel)
                    {
                        int32_t nProf = Player::getProf(m_pPlayer);
                        int32_t nItemType = pShop->m_ItemType;
                        if (nItemType == 0 || nItemType == nProf)
                        {
                            if (!pShop->m_FunctionID || CFunctionOpen::IsOpenByPlayer(pFuncOpen, m_pPlayer, pShop->m_FunctionID))
                            {
                                ++nCount;
                                packet->writeInt32(pShop->m_Id);
                                packet->writeInt32(pShop->m_ItemId);
                                packet->writeInt32(pShop->m_ItemCount);
                                packet->writeInt8(pShop->m_ItemClass);
                                packet->writeInt32(pShop->m_PriceType);
                                packet->writeInt32(pShop->m_Price);
                                packet->writeInt8(pShop->m_ShopType);
                                packet->writeInt8(pShop->m_ItemBind);
                                packet->writeInt8(pShop->m_Count > 0 ? 1 : 0);
                                if (pShop->m_Count > 0)
                                {
                                    auto it2 = m_ShopItem.find(pShop->m_Id);
                                    if (it2 != m_ShopItem.end())
                                        packet->writeInt32(it2->second);
                                    else
                                        packet->writeInt32(0);
                                }
                            }
                        }
                    }
                }
            }
            CfgShopTable::Iterator::next(it);
        } while (CfgShopTable::Iterator::isValid(it));
        CfgShopTable::Iterator::free(it);
    }
    uint32_t nEndPos = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nCountPos);
    packet->writeInt32(nCount);
    Answer::NetPacket::setWOffset(packet, nEndPos);
    Answer::NetPacket::setSize(packet, nEndPos);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CShangCheng::OnGetBaoKuData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D8D);
    if (!packet)
        return 2;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::SendBaokuData(pBag, m_pPlayer, packet);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CShangCheng::OnPickUpAllItems(Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::TakeAllBaoku(pBag, m_pPlayer);
    return 0;
}

int32_t CShangCheng::OnPickUpOneItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGuid = inPacket->readInt32();
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::TakeOneBaoku(pBag, nGuid);
    return 0;
}

int32_t CShangCheng::OnGetBaoKuLog(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D8E);
    if (!packet)
        return 2;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::SendBaokuLog(pBag, m_pPlayer, packet);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CShangCheng::OnBroadcastBuyShopItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t ShopId = inPacket->readInt32();
    BroadcastBuyShopItem(ShopId, 1);
    return 0;
}

void CShangCheng::BroadcastBuyShopItem(int32_t ShopId, int32_t IsSaveToDB)
{
    if (!m_pPlayer)
        return;
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const CfgShopTable* pShopTable = CfgData::GetShopTable(v3);
    const CfgShop* pCfgShop = CfgShopTable::GetRow(pShopTable, ShopId);
    if (!pCfgShop || pCfgShop->m_Broadcast <= 0)
        return;
    auto it = m_broadcastBuyItemList.find(ShopId);
    if (it != m_broadcastBuyItemList.end() && it->second > 0)
        return;
    m_broadcastBuyItemList[ShopId] += pCfgShop->m_Broadcast;
    if (IsSaveToDB == 1)
    {
        DBService* v5 = Answer::Singleton<DBService>::instance();
        int8_t ConnId = Player::getConnId(m_pPlayer);
        DBService::OnBroadcastBuyShopItem(v5, ConnId, ShopId);
    }
    MString playerName;
    Player::getPlayerName(m_pPlayer, &playerName);
    GameService* v7 = Answer::Singleton<GameService>::instance();
    if (pCfgShop->m_Broadcast == 3)
    {
        GameService::SendMarqueeServerNotice(v7, playerName, ShopId, 1);
    }
    else if (pCfgShop->m_Broadcast == 1 || pCfgShop->m_Broadcast == 2 || pCfgShop->m_Broadcast == 4)
    {
        GameService::SendMarqueeMessage(v7, playerName, ShopId, 1);
    }
}

int32_t CShangCheng::OnGetShangRenIcon(Answer::NetPacket* inPacket)
{
    return 0;
}

int32_t CShangCheng::OnTestShop(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int32_t ShopId = inPacket->readInt32();
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgShopTable* pShopTable = CfgData::GetShopTable(v4);
    const CfgShop* pCfgShop = CfgShopTable::GetRow(pShopTable, ShopId);
    if (!pCfgShop)
        return 2;
    int32_t nCount = pCfgShop->m_Count;
    if (nCount > 0)
    {
        auto it = m_ShopItem.find(ShopId);
        if (it != m_ShopItem.end())
        {
            int32_t nDelta = it->second + 1;
            if (nDelta > nCount)
                return 2;
        }
    }
    Player::AddCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nCount, CURRENCY_CHANGE_REASON::GCR_MAX, ShopId);
    uint16_t v9 = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v11 = Player::getConnId(m_pPlayer);
    GameService* v12 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v12, v11, GateIndex, v9, 0);
    return 0;
}

int32_t CShangCheng::OnS2CGMTool(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int32_t ShopId = inPacket->readInt32();
    int32_t nCount = inPacket->readInt32();
    m_ShopItem[ShopId] = nCount;
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v7 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v7, ConnId, GateIndex, Proc, 0);
    return 0;
}

void CShangCheng::AddLimitCount(int32_t ShopId, int32_t AddCount)
{
    if (ShopId > 0 && AddCount > 0)
    {
        auto it = m_ShopItem.find(ShopId);
        if (it != m_ShopItem.end())
        {
            it->second += AddCount;
        }
        else
        {
            m_ShopItem[ShopId] = AddCount;
        }
    }
}

void CShangCheng::SendResetLimitInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D05u);
    if (packet)
    {
        packet->writeInt32(0);
        uint32_t WOffset = packet->getWOffset();
        packet->setSize(WOffset);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
    }
}

void CShangCheng::SendItemLimitChange(int32_t ShopId)
{
    auto it = m_ShopItem.find(ShopId);
    if (it != m_ShopItem.end())
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CAFu);
        if (packet)
        {
            packet->writeInt32(1);
            packet->writeInt32(it->first);
            packet->writeInt32(it->second);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
        }
    }
}

int32_t CShangCheng::OnAskLimitInfo(Answer::NetPacket* inPacket)
{
    SendLimitInfo();
    SendSocialAskLimitInfo();
    return 0;
}

void CShangCheng::SendSocialAskLimitInfo()
{
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* Packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x4EB2u);
    if (Packet && m_pPlayer)
    {
        int32_t GateIndex = Player::getGateIndex(m_pPlayer);
        Packet->writeInt32(GateIndex);
        Packet->writeInt8(0);
        uint32_t WOffset = Packet->getWOffset();
        Packet->setSize(WOffset);
        GameService::sendPacket(gs, Player::getConnId(m_pPlayer), Packet);
    }
}

void CShangCheng::SendSocialBuyLimitItem(int32_t ShopId, int32_t Count)
{
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* Packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x4EB3u);
    if (Packet && m_pPlayer)
    {
        int32_t GateIndex = Player::getGateIndex(m_pPlayer);
        Packet->writeInt32(GateIndex);
        Packet->writeInt32(ShopId);
        Packet->writeInt32(Count);
        uint32_t WOffset = Packet->getWOffset();
        Packet->setSize(WOffset);
        GameService::sendPacket(gs, Player::getConnId(m_pPlayer), Packet);
    }
}

bool CShangCheng::IsLimited(CfgGameShop* pShangCheng, int32_t ItemCount)
{
    if (!pShangCheng || ItemCount <= 0 || !m_pPlayer)
        return true;
    int32_t LimitType = pShangCheng->LimitType;
    if (LimitType == 1)
        return ItemCount + GetLimitCount(pShangCheng->ShopId) > pShangCheng->LimitCount;
    if (LimitType != 3)
        return LimitType != 0;
    int32_t TimeNow = Unit::getNow(m_pPlayer);
    return pShangCheng->LimitStartTime >= TimeNow || pShangCheng->LimitEndTime <= TimeNow;
}

void CShangCheng::SendLimitInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CAFu);
    if (packet)
    {
        int32_t nSize = m_ShopItem.size();
        packet->writeInt32(nSize);
        for (auto& kv : m_ShopItem)
        {
            packet->writeInt32(kv.first);
            packet->writeInt32(kv.second);
        }
        uint32_t WOffset = packet->getWOffset();
        packet->setSize(WOffset);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
    }
}

int32_t CShangCheng::OnBuyGameLimitItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t ShopId = inPacket->readInt32();
    int32_t ItemCount = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    CfgGameShop* pShangCheng = CfgData::GetGameShop(cfg, ShopId);
    if (!pShangCheng || ItemCount <= 0)
        return 2;
    int32_t nLayNum = CfgData::getOverlay(cfg, pShangCheng->ItemId, pShangCheng->ItemClass);
    if (ItemCount > nLayNum)
        return 2;
    if (pShangCheng->LimitType != 2)
        return 2;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::GetFreeSlotCount(Bag) <= 0)
        return 2;
    if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < ItemCount * pShangCheng->Price)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, ItemCount * pShangCheng->Price,
                             CURRENCY_CHANGE_REASON::GCR_BUY_SHANG_CHENG_ITEM, pShangCheng->ShopId))
        return 2;
    MemChrBag Item;
    memset(&Item, 0, sizeof(Item));
    Item.itemClass = pShangCheng->ItemClass;
    Item.itemCount = ItemCount;
    Item.bind = pShangCheng->ItemBind;
    Item.itemId = pShangCheng->ItemId;
    MemChrBagVector items;
    items.push_back(Item);
    if (!CExtCharBag::AddItemsAndMingGe(Bag, &items, ITEM_CHANGE_REASON::ICR_BUY_SHANG_CHENG_ITEM))
    {
        CharId_t Cid = Player::getCid(m_pPlayer);
        unsigned int Now = Unit::getNow(m_pPlayer);
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "buy_item",
                              (unsigned int)pShangCheng->ItemClass,
                              (unsigned int)pShangCheng->ItemId,
                              (unsigned int)ItemCount, Now, Cid);
    }
    SendBuyItemSucceed(ShopId, ItemCount);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, ConnId, GateIndex, 0x1B2u, pShangCheng->ShopId);
    return 0;
}

int32_t CShangCheng::BuyShopItem(int32_t ShopId, int32_t ItemCount, bool AddBag)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    CfgGameShop* pShop = CfgData::GetGameShop(cfg, ShopId);
    if (!pShop)
        return 2;
    if (ItemCount <= 0)
        return 2;
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    int32_t VipLevel = pVip->GetVipLevel();
    if (VipLevel < pShop->VipLevelLimit)
        return 2;
    if (pShop->LimitType == 2)
    {
        SendSocialBuyLimitItem(ShopId, ItemCount);
        return 0;
    }
    int32_t nLayNum = CfgData::getOverlay(cfg, pShop->ItemId, pShop->ItemClass);
    if (ItemCount > nLayNum)
        return 2;
    if (pShop->CostType == 2)
    {
        if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_CASH) < ItemCount * pShop->Price)
            return 2;
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_CASH, ItemCount * pShop->Price,
                                 CURRENCY_CHANGE_REASON::GCR_BUY_SHANG_CHENG_ITEM, ShopId))
            return 2;
    }
    else if (pShop->CostType == 1)
    {
        if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < ItemCount * pShop->Price)
            return 2;
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, ItemCount * pShop->Price,
                                 CURRENCY_CHANGE_REASON::GCR_BUY_SHOP_ITEM, ShopId))
            return 2;
    }
    if (AddBag)
    {
        MemChrBag Item;
        memset(&Item, 0, sizeof(Item));
        Item.itemClass = pShop->ItemClass;
        Item.itemId = pShop->ItemId;
        Item.itemCount = ItemCount;
        Item.bind = pShop->ItemBind;
        MemChrBagVector items;
        items.push_back(Item);
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::AddItemsAndMingGe(Bag, &items, ITEM_CHANGE_REASON::ICR_BUY_ITEM))
            return 2;
    }
    AddLimitCount(ShopId, ItemCount);
    SendItemLimitChange(ShopId);
    return 0;
}

void CShangCheng::SendBuyItemSucceed(int32_t ShopId, int32_t AddCount)
{
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* Packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x4EB4u);
    if (Packet && m_pPlayer)
    {
        int32_t GateIndex = Player::getGateIndex(m_pPlayer);
        Packet->writeInt32(GateIndex);
        Packet->writeInt32(ShopId);
        Packet->writeInt32(AddCount);
        uint32_t WOffset = Packet->getWOffset();
        Packet->setSize(WOffset);
        GameService::sendPacket(gs, Player::getConnId(m_pPlayer), Packet);
    }
}

int32_t CShangCheng::GetLimitCount(int32_t ShopId)
{
    auto it = m_ShopItem.find(ShopId);
    if (it != m_ShopItem.end())
        return it->second;
    return 0;
}

