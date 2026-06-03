#include "common.h"
#include "Other/CBlackMarket.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtOperateLimit.h"

CBlackMarket::CBlackMarket()
    : CExtSystemBase()
{
}

CBlackMarket::~CBlackMarket()
{
}

void CBlackMarket::OnCleanUp()
{
}

void CBlackMarket::OnUpdate(int64_t curTick)
{
}

void CBlackMarket::OnDaySwitch(int32_t nDiffDays)
{
}

void CBlackMarket::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CBlackMarket::OnSaveToDB(PlayerDBData* dbData)
{
}

void CBlackMarket::GetInterestsProtocol(ProcIdList* const procList)
{
    procList->push_back(956);
    procList->push_back(957);
    procList->push_back(958);
}

int32_t CBlackMarket::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 0x3BD:
        return onEnterMarket(inPacket);
    case 0x3BE:
        return onBuyMarketGoods(inPacket);
    case 0x3BC:
        return onRequestMarketInfo(inPacket);
    }
    return 0;
}

int32_t CBlackMarket::onRequestMarketInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    sendMarketInfo();
    return 0;
}

int32_t CBlackMarket::onEnterMarket(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(m_pPlayer);
    if (CExtOperateLimit::GetLimitCount(OperateLimit, 2041) > 0)
        return 10002;
    int32_t nSlot = inPacket->readInt32();
    if (nSlot >= 0)
    {
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        MemChrBag bagSlot = *Bag->GetSlotData(nSlot);
        if (bagSlot.itemClass != 1 || bagSlot.itemCount <= 0)
            return 10002;
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        CfgItem* pCfgItem = CfgData::getItem(cfg, bagSlot.itemId);
        if (!pCfgItem || pCfgItem->type != 167)
            return 10002;
        --bagSlot.itemCount;
        Bag->SetSlotData(nSlot, &bagSlot, ITEM_CHANGE_REASON::ICR_BLACK_MARKET_ENTER_TICKET, 0);
    }
    else if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, 100, CURRENCY_CHANGE_REASON::GCR_BLACK_MARCKET_ENTER_TICKET, 0))
    {
        return 10002;
    }
    Player::GetOperateLimit(m_pPlayer)->UpdateLimitCount(2041, 1);
    uint16_t Proc = inPacket->getProc();
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, Proc, 1);
    return 0;
}

int32_t CBlackMarket::onBuyMarketGoods(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 2;
    int8_t nIndex = inPacket->readInt8();
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(m_pPlayer);
    if (CExtOperateLimit::GetLimitCount(OperateLimit, 2041) <= 0)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nDay = CfgData::getServerDiffDay(cfg, SERVER_TYPE::SVT_NORMAL);
    Job_t Job = Player::getJob(m_pPlayer);
    const CfgBlacketMarketTable* BlacketMarketTable = CfgData::GetBlacketMarketTable(cfg);
    const CfgBlackMarketGoods* pCfgGoods = CfgBlacketMarketTable::GetGoods(BlacketMarketTable, nDay + 1, nIndex + 1, Job);
    if (!pCfgGoods)
        return 2;
    int32_t nOldRecord = Player::GetOperateLimit(m_pPlayer)->GetLimitCount(2042);
    int nNewRecord = nOldRecord | (1 << nIndex);
    if (nOldRecord == nNewRecord)
        return 2;
    if (pCfgGoods->nPrice <= 0 || Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < pCfgGoods->nPrice)
        return 2;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfgGoods->item, ITEM_CHANGE_REASON::ICR_BLACK_MARKET_BUY_GOODS))
        return 2;
    Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfgGoods->nPrice, CURRENCY_CHANGE_REASON::GCR_BLACK_MARCKET_BUY_GOODS, 0);
    Player::updateRecord(m_pPlayer, 2042, nNewRecord);
    if (pCfgGoods->nBroadcast > 0)
        broadcastBuyItem(pCfgGoods->nBroadcast, &pCfgGoods->item);
    SendMarketIcon();
    uint16_t Proc = inPacket->getProc();
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, Proc, nIndex);
    return 0;
}

void CBlackMarket::sendMarketInfo()
{
    if (!m_pPlayer)
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nDay = CfgData::getServerDiffDay(cfg, SERVER_TYPE::SVT_NORMAL);
    Job_t Job = Player::getJob(m_pPlayer);
    const CfgBlacketMarketTable* BlacketMarketTable = CfgData::GetBlacketMarketTable(cfg);
    if (!CfgBlacketMarketTable::GetGoods(BlacketMarketTable, nDay + 1, 1, Job))
        return;
    int32_t nRecord = Player::GetOperateLimit(m_pPlayer)->GetLimitCount(2042);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2F18);
    if (!packet)
        return;
    packet->writeInt8(static_cast<int8_t>(Player::GetOperateLimit(m_pPlayer)->GetLimitCount(2041)));
    packet->writeInt8(4);
    for (int8_t i = 0; i <= 3; ++i)
        packet->writeInt8(static_cast<int8_t>((nRecord >> i) & 1));
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

void CBlackMarket::SendMarketIcon()
{
    if (!m_pPlayer)
        return;
    ShowIcon stu;
    getMarketIconStu(&stu);
    Player::SendIconState(m_pPlayer, &stu);
}

void CBlackMarket::GetMarketIconState(IconStateList* const IconList)
{
    if (!m_pPlayer)
        return;
    if (checkBuyRecord())
    {
        ShowIcon icon;
        getMarketIconStu(&icon);
        IconList->push_back(icon);
    }
}

void CBlackMarket::getMarketIconStu(ShowIcon* __return_ptr retstr)
{
    retstr->nId = 0;
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
    retstr->nId = 57;
    retstr->nState = checkBuyRecord() ? 2 : 4;
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
}

bool CBlackMarket::checkBuyRecord()
{
    if (!m_pPlayer)
        return false;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::isUniteServer(cfg))
        return false;
    int32_t nDay = CfgData::getServerDiffDay(cfg, SERVER_TYPE::SVT_NORMAL);
    Job_t Job = Player::getJob(m_pPlayer);
    const CfgBlacketMarketTable* BlacketMarketTable = CfgData::GetBlacketMarketTable(cfg);
    if (!CfgBlacketMarketTable::GetGoods(BlacketMarketTable, nDay + 1, 1, Job))
        return false;
    int32_t nRecord = 0;
    for (int8_t i = 0; i <= 3; ++i)
        nRecord |= 1 << i;
    return Player::GetOperateLimit(m_pPlayer)->GetLimitCount(2042) != nRecord;
}

void CBlackMarket::broadcastBuyItem(int32_t nBroadcast, const MemChrBag* const item)
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (!packet)
        return;
    packet->writeInt32(nBroadcast);
    std::string val;
    Player::getName(m_pPlayer, &val);
    packet->writeUTF8(&val);
    packet->writeInt64(Player::getCid(m_pPlayer));
    packet->writeInt32(item->itemId);
    packet->writeInt8(item->itemClass);
    packet->writeInt32(item->itemCount);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::worldBroadcast(gs2, connId2, packet);
}
