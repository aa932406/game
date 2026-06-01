#include "Other/CChrDepot.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"

CChrDepot::CChrDepot()
    : CExtSystemBase()
{
    OnCleanUp();
}

CChrDepot::~CChrDepot()
{
}

void CChrDepot::OnCleanUp()
{
    m_DepotBag.Clear();
    m_DepotPwd.clear();
    m_nDepotCellCount = 0;
    m_bDepotLock = false;
    m_bPwdCheckPass = false;
}

void CChrDepot::OnUpdate(int64_t curTick)
{
}

void CChrDepot::OnDaySwitch(int32_t nDiffDays)
{
}

void CChrDepot::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_DepotBag = dbData->m_DepotData.m_DepotBag;
    m_DepotPwd = dbData->m_DepotData.m_DepotPwd;
    m_nDepotCellCount = dbData->m_DepotData.m_nDepotCellCount;
    m_bDepotLock = dbData->m_DepotData.m_bDepotLock;
}

void CChrDepot::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_DepotData.m_DepotBag = m_DepotBag;
    dbData->m_DepotData.m_DepotPwd = m_DepotPwd;
    dbData->m_DepotData.m_nDepotCellCount = m_nDepotCellCount;
    dbData->m_DepotData.m_bDepotLock = m_bDepotLock;
}

void CChrDepot::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(502);
    procList->push_back(503);
    procList->push_back(504);
    procList->push_back(505);
    procList->push_back(506);
    procList->push_back(507);
    procList->push_back(508);
}

int32_t CChrDepot::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 502:
        return OnDepotOpr(inPacket);
    case 503:
        return OnDepotPwdCheck(inPacket);
    case 504:
        return OnDepotPwdSetting(inPacket);
    case 505:
        return OnDepotSwitch(inPacket);
    case 506:
        return OnDepotInfo(inPacket);
    case 507:
        return OnDepotPLock(inPacket);
    case 508:
        return OnDepotPLock(inPacket);
    }
    return 0;
}

int32_t CChrDepot::OnDepotOpr(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nOpType = inPacket->readInt32();
    if (nOpType == 0)
    {
        int32_t nGrid = inPacket->readInt32();
        int32_t nItemId = inPacket->readInt32();
        int32_t nCount = inPacket->readInt32();
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        ItemInfo* pItem = Bag->GetItem(nGrid);
        if (!pItem)
            return 10002;
        ItemInfo item = *pItem;
        if (!Bag->RemoveItem(nGrid, nCount, ITEM_CHANGE_REASON::ICR_DEPOT_OPR))
            return 10002;
        if (!m_DepotBag.AddItem(&item, nCount, ITEM_CHANGE_REASON::ICR_DEPOT_OPR))
        {
            Bag->AddItem(&item, nCount, ITEM_CHANGE_REASON::ICR_DEPOT_OPR);
            return 10002;
        }
    }
    else if (nOpType == 1)
    {
        int32_t nGrid = inPacket->readInt32();
        int32_t nCount = inPacket->readInt32();
        ItemInfo* pItem = m_DepotBag.GetItem(nGrid);
        if (!pItem)
            return 10002;
        ItemInfo item = *pItem;
        if (!m_DepotBag.RemoveItem(nGrid, nCount, ITEM_CHANGE_REASON::ICR_DEPOT_OPR))
            return 10002;
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!Bag->AddItem(&item, nCount, ITEM_CHANGE_REASON::ICR_DEPOT_OPR))
        {
            m_DepotBag.AddItem(&item, nCount, ITEM_CHANGE_REASON::ICR_DEPOT_OPR);
            return 10002;
        }
    }
    else if (nOpType == 2)
    {
        int32_t nGrid = inPacket->readInt32();
        m_DepotBag.RemoveItem(nGrid, m_DepotBag.GetItemCount(nGrid), ITEM_CHANGE_REASON::ICR_DEPOT_OPR);
    }
    SendDepotItems();
    return 0;
}

int32_t CChrDepot::OnDepotPwdCheck(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    std::string pwd = inPacket->readUTF8();
    if (!CheckPwd(&pwd))
        return 10002;
    m_bPwdCheckPass = true;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, Player::getGateIndex(m_pPlayer), inPacket->getProc(), 0);
    return 0;
}

int32_t CChrDepot::OnDepotPwdSetting(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    std::string oldPwd = inPacket->readUTF8();
    std::string newPwd = inPacket->readUTF8();
    if (!m_DepotPwd.empty() && !CheckPwd(&oldPwd))
        return 10002;
    SetDepotPwd(&newPwd);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, Player::getGateIndex(m_pPlayer), inPacket->getProc(), 0);
    return 0;
}

int32_t CChrDepot::OnDepotSwitch(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    bool bLock = inPacket->readInt8() != 0;
    SetDepotLockState(bLock);
    return 0;
}

int32_t CChrDepot::OnDepotInfo(Answer::NetPacket* inPacket)
{
    SendDepotInfo();
    return 0;
}

int32_t CChrDepot::OnDepotPLock(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    std::string pwd = inPacket->readUTF8();
    if (pwd != m_DepotPwd)
        return 10002;
    m_bDepotLock = false;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, Player::getGateIndex(m_pPlayer), inPacket->getProc(), 0);
    return 0;
}

void CChrDepot::SendDepotItems()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CEF);
    if (!packet)
        return;
    m_DepotBag.PacketAllItems(packet);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CChrDepot::SendDepotInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CF0);
    if (!packet)
        return;
    packet->writeInt32(GetDepotCellNum());
    packet->writeInt32(GetDepotExpandCount());
    packet->writeInt8(m_bDepotLock ? 1 : 0);
    packet->writeInt8(m_DepotPwd.empty() ? 0 : 1);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

int32_t CChrDepot::GetDepotCellNum()
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    return CfgData::GetDepotInitCell(cfg) + m_nDepotCellCount;
}

int32_t CChrDepot::GetDepotExpandCount()
{
    return m_nDepotCellCount;
}

bool CChrDepot::CheckPwd(std::string* pwd)
{
    return *pwd == m_DepotPwd;
}

void CChrDepot::SetDepotPwd(std::string* pwd)
{
    m_DepotPwd = *pwd;
}

bool CChrDepot::ItemCanEnterDepot(ItemInfo* item)
{
    if (!item)
        return false;
    return !item->IsBind();
}

void CChrDepot::ExpandDepotCell()
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nPrice = CfgData::GetDepotExpandPrice(cfg, m_nDepotCellCount);
    if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < nPrice)
        return;
    Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nPrice, CURRENCY_CHANGE_REASON::GCR_DEPOT_EXPAND, 0);
    ++m_nDepotCellCount;
    SendDepotInfo();
}

void CChrDepot::SetDepotLockState(bool bLock)
{
    if (!bLock && !m_DepotPwd.empty() && m_bDepotLock)
    {
        m_bDepotLock = false;
        return;
    }
    if (bLock && !m_DepotPwd.empty())
    {
        m_bDepotLock = true;
        return;
    }
}
