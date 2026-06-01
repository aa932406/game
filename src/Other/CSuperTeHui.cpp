#include "Other/CSuperTeHui.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"

CSuperTeHui::CSuperTeHui()
    : CExtSystemBase()
{
    m_TeHuiItem.clear();
}

CSuperTeHui::~CSuperTeHui()
{
}

void CSuperTeHui::OnCleanUp()
{
}

void CSuperTeHui::OnUpdate(int64_t curTick)
{
}

void CSuperTeHui::OnDaySwitch(int32_t nDiffDays)
{
}

void CSuperTeHui::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_TeHuiItem = dbData->m_SuperTeHuiDBData.m_TeHuiItem;
}

void CSuperTeHui::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_SuperTeHuiDBData.m_TeHuiItem = m_TeHuiItem;
}

void CSuperTeHui::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(411);
    procList->push_back(412);
}

int32_t CSuperTeHui::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 411)
        return OnGetTeHuiData(inPacket);
    if (nProcId == 412)
        return OnBuyTeHuiItem(inPacket);
    return 2;
}

int32_t CSuperTeHui::OnGetTeHuiData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2C96);
    if (!packet)
        return 2;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgTeHuiTable* pTable = CfgData::GetTeHuiTable(v4);
    int32_t nCurTime = Unit::getNow(m_pPlayer);
    int32_t nCount = 0;
    uint32_t nCountPos = Answer::NetPacket::getWOffset(packet);
    packet->writeInt32(0);
    CfgTeHuiTable::Iterator* it = CfgTeHuiTable::GetIterator(pTable);
    if (it)
    {
        do
        {
            const CfgTeHui* pCfg = CfgTeHuiTable::Iterator::getData(it);
            if (pCfg)
            {
                if (pCfg->nStartTime <= nCurTime && pCfg->nEndTime >= nCurTime)
                {
                    if (nCount < 100)
                    {
                        ++nCount;
                        packet->writeInt32(pCfg->nId);
                        packet->writeInt32(pCfg->nItemId);
                        packet->writeInt32(pCfg->nItemCount);
                        packet->writeInt32(pCfg->nPrice);
                        packet->writeInt32(pCfg->nPriceType);
                        packet->writeInt8(pCfg->nItemClass);
                        packet->writeInt32(pCfg->m_Discount);
                        if (m_TeHuiItem.find(pCfg->nId) != m_TeHuiItem.end())
                        {
                            int32_t nBuyCount = m_TeHuiItem[pCfg->nId];
                            packet->writeInt32(nBuyCount);
                        }
                        else
                        {
                            packet->writeInt32(0);
                        }
                    }
                }
            }
            CfgTeHuiTable::Iterator::next(it);
        } while (CfgTeHuiTable::Iterator::isValid(it));
        CfgTeHuiTable::Iterator::free(it);
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

int32_t CSuperTeHui::OnBuyTeHuiItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int32_t nTeHuiId = inPacket->readInt32();
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgTeHuiTable* pTable = CfgData::GetTeHuiTable(v4);
    const CfgTeHui* pCfg = CfgTeHuiTable::GetRow(pTable, nTeHuiId);
    if (!pCfg)
        return 2;
    int32_t nCurTime = Unit::getNow(m_pPlayer);
    if (nCurTime < pCfg->nStartTime || nCurTime > pCfg->nEndTime)
        return 2;
    int32_t nLimitCount = pCfg->nLimitCount;
    if (nLimitCount > 0)
    {
        auto it = m_TeHuiItem.find(nTeHuiId);
        if (it != m_TeHuiItem.end() && it->second >= nLimitCount)
            return 2;
    }
    int32_t nPriceType = pCfg->nPriceType;
    int32_t nPrice = pCfg->nPrice;
    if (nPriceType == 1)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_TEHUI_BUY, 0))
            return 2;
    }
    else if (nPriceType == 2)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_TEHUI_BUY, 0))
            return 2;
    }
    else if (nPriceType == 5)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_TEHUI_BUY, 0))
            return 2;
    }
    else if (nPriceType == 6)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_TEHUI_BUY, 0))
            return 2;
    }
    else if (nPriceType == 7)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_TEHUI_BUY, 0))
            return 2;
    }
    int32_t nItemClass = pCfg->nItemClass;
    int32_t nItemId = pCfg->nItemId;
    int32_t nItemCount = pCfg->nItemCount;
    MemChrBagVector ItemList;
    ItemList.reserve(1);
    MemChrBag item;
    memset(&item, 0, sizeof(item));
    item.itemClass = nItemClass;
    item.itemId = nItemId;
    item.itemCount = nItemCount;
    item.bind = 0;
    ItemList.push_back(item);
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItemsAndMingGe(pBag, &ItemList, ITEM_CHANGE_REASON::ICR_TEHUI_BUY))
        return 2;
    m_TeHuiItem[nTeHuiId] += 1;
    GameService* v15 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v15, Player::getConnId(m_pPlayer), Player::getGateIndex(m_pPlayer), Proc, 0);
    return 0;

bool CSuperTeHui::IsAllGetTeHui()
{
    if (!m_pPlayer)
        return true;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 1028);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const SuperTeHuiCfgMap* pCfgMap = CfgData::GetSuperTeHuiCfgMap(pCfgData);
    for (auto it = pCfgMap->begin(); it != pCfgMap->end(); ++it)
    {
        int32_t NewRecord = OldRecord | (1 << (it->first - 1));
        if (OldRecord != NewRecord)
            return false;
    }
    return true;
}

void CSuperTeHui::GetSuperTeHuiIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 201))
    {
        ShowIcon icon;
        GetSuperTeHuiIcon(&icon);
        IconList->push_back(icon);
    }
}

void CSuperTeHui::SendSuperTeHuiIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 201))
    {
        ShowIcon icon;
        GetSuperTeHuiIcon(&icon);
        Player::SendIconState(m_pPlayer, &icon);
    }
}

void CSuperTeHui::GetSuperTeHuiIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (!m_pPlayer)
        return;
    retstr->nId = 121;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 201) && !IsAllGetTeHui())
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nEffects = 1;
}

int32_t CSuperTeHui::OnGetJewelPavilionItem(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    if (!IsOpenedJewelPavilion())
        return 10002;
    int32_t nDay = GetJewelPavilionDay();
    int32_t nIndex = inPacket->readInt32();
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    JewelPavilionCfg* pCfg = CfgData::GetJewelPavilionCfg(pCfgData, nDay, nIndex);
    if (!pCfg)
        return 10002;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 2085);
    int32_t NewRecord = OldRecord | (1 << (nIndex - 1));
    if (OldRecord == NewRecord)
        return 2;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::GetFreeSlotCount(pBag) <= 0)
        return 10002;
    if (pCfg->nPrice > 0 && !Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->nPrice, CURRENCY_CHANGE_REASON::GCR_JEWEL_PAVILION, 0))
        return 10002;
    pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::AddItem(pBag, &pCfg->Item, ITEM_CHANGE_REASON::ICR_JEWEL_PAVILION);
    Player::updateRecord(m_pPlayer, 2085, NewRecord);
    SendJewelPavilionIcon();
    return 0;
}

void CSuperTeHui::GetJewelPavilionIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    if (IsOpenedJewelPavilion())
    {
        ShowIcon icon;
        GetJewelPavilionIcon(&icon);
        IconList->push_back(icon);
    }
}

void CSuperTeHui::SendJewelPavilionIcon()
{
    if (!m_pPlayer)
        return;
    if (IsOpenedJewelPavilion())
    {
        ShowIcon icon;
        GetJewelPavilionIcon(&icon);
        Player::SendIconState(m_pPlayer, &icon);
    }
}

void CSuperTeHui::GetJewelPavilionIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (!m_pPlayer)
        return;
    retstr->nId = 122;
    if (IsOpenedJewelPavilion() && !IsTimeEnd())
    {
        if (GetJewelPavilionDay() == 7 && IsAllGetPavilion())
            retstr->nState = 4;
        else
            retstr->nState = 2;
    }
    else
    {
        retstr->nState = 4;
    }
    if (!IsAllGetPavilion())
        retstr->nEffects = 1;
    retstr->nIconRight = HaveFreeCount();
}

int32_t CSuperTeHui::HaveFreeCount()
{
    int32_t Count = 0;
    int32_t nDay = GetJewelPavilionDay();
    int32_t OldRecord = Player::getRecord(m_pPlayer, 2085);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const JewelPavilionCfgMap* pCfgMap = CfgData::GetJewelPavilionCfgMap(pCfgData);
    for (auto it = pCfgMap->begin(); it != pCfgMap->end(); ++it)
    {
        if (it->first.first == nDay && it->second.nPrice <= 0)
        {
            int32_t NewRecord = OldRecord | (1 << (it->first.second - 1));
            if (OldRecord != NewRecord)
                ++Count;
        }
    }
    return Count;
}

bool CSuperTeHui::IsAllGetPavilion()
{
    if (!m_pPlayer)
        return true;
    int32_t nDay = GetJewelPavilionDay();
    int32_t OldRecord = Player::getRecord(m_pPlayer, 2085);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const JewelPavilionCfgMap* pCfgMap = CfgData::GetJewelPavilionCfgMap(pCfgData);
    for (auto it = pCfgMap->begin(); it != pCfgMap->end(); ++it)
    {
        if (it->first.first == nDay)
        {
            int32_t NewRecord = OldRecord | (1 << (it->first.second - 1));
            if (OldRecord != NewRecord)
                return false;
        }
    }
    return true;
}

bool CSuperTeHui::IsTimeEnd()
{
    if (!m_pPlayer)
        return true;
    int32_t StartTime = Player::getRecord(m_pPlayer, 1034);
    int32_t EndTime = Answer::DayTime::dayzero(StartTime) + 604800;
    return Unit::getNow(m_pPlayer) > EndTime;
}

void CSuperTeHui::SetJewelPavilionOpen()
{
    if (!m_pPlayer)
        return;
    int32_t Now = Unit::getNow(m_pPlayer);
    Player::updateRecord(m_pPlayer, 1034, Now);
    SendJewelPavilionIcon();
}

int32_t CSuperTeHui::GetJewelPavilionDay()
{
    if (!m_pPlayer)
        return 0;
    int32_t StartTime = Player::getRecord(m_pPlayer, 1034);
    return Answer::DayTime::daydiff(Answer::DayTime::dayzero(StartTime)) + 1;
}

int32_t CSuperTeHui::OnBuyShengRenItem(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t Day = CfgData::getServerDiffDay(pCfgData, SERVER_TYPE::SVT_NORMAL) + 1;
    if (Day <= 9 || Day > 12)
        return 10002;
    int32_t nId = inPacket->readInt32();
    ShangRenCfg* pShangRenCfg = CfgData::GetShangRenCfg(pCfgData, nId);
    if (!pShangRenCfg)
        return 10002;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 2100);
    int32_t NewRecord = OldRecord | (1 << (nId - 1));
    if (OldRecord == NewRecord)
        return 2;
    if (pShangRenCfg->vItem.empty())
        return 2;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::GetFreeSlotCount(pBag) < (int32_t)pShangRenCfg->vItem.size())
        return 2;
    if (pShangRenCfg->nPrice <= 0)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pShangRenCfg->nPrice, CURRENCY_CHANGE_REASON::GCR_SHANG_REN, nId))
        return 2;
    pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::AddItem(pBag, &pShangRenCfg->vItem, ITEM_CHANGE_REASON::IDCR_SHANG_REN);
    Player::updateRecord(m_pPlayer, 2100, NewRecord);
    SendShangRenIcon();
    return 0;
}

void CSuperTeHui::GetShangRenIcon(IconStateList* IconList)
{
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t Day = CfgData::getServerDiffDay(pCfgData, SERVER_TYPE::SVT_NORMAL) + 1;
    if (Day > 9 && Day <= 12)
    {
        ShowIcon icon;
        GetShangRenIcon(&icon);
        IconList->push_back(icon);
    }
}

void CSuperTeHui::SendShangRenIcon()
{
    if (!m_pPlayer)
        return;
    ShowIcon icon;
    GetShangRenIcon(&icon);
    Player::SendIconState(m_pPlayer, &icon);
}

void CSuperTeHui::GetShangRenIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (!m_pPlayer)
        return;
    retstr->nId = 131;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t Day = CfgData::getServerDiffDay(pCfgData, SERVER_TYPE::SVT_NORMAL) + 1;
    if (Day > 9 && Day <= 12 && !IsSoldOut())
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nEffects = 1;
    retstr->nLeftTime = GetShangRenLeftTime();
}

int32_t CSuperTeHui::GetShangRenLeftTime()
{
    if (!m_pPlayer)
        return 2;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t Day = CfgData::getServerDiffDay(pCfgData, SERVER_TYPE::SVT_NORMAL) + 1;
    if (Day <= 9 || Day > 12)
        return 0;
    int32_t EndTime = CfgData::getServerStartDayZeroTime(pCfgData, SERVER_TYPE::SVT_NORMAL) + 1036800;
    return EndTime - Unit::getNow(m_pPlayer);
}

void CSuperTeHui::SendLeftTime()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CC6);
    if (!packet)
        return;
    packet->writeInt32(GetShangRenLeftTime());
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

bool CSuperTeHui::IsSoldOut()
{
    if (!m_pPlayer)
        return true;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 2100);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const ShangRenCfgMap* pCfgMap = CfgData::GetShangRenCfgMap(pCfgData);
    for (auto it = pCfgMap->begin(); it != pCfgMap->end(); ++it)
    {
        int32_t NewRecord = OldRecord | (1 << (it->first - 1));
        if (OldRecord != NewRecord)
            return false;
    }
    return true;
}
