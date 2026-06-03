#include "common.h"
#include "Other/CGoblin.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"

CGoblin::CGoblin()
    : CExtSystemBase()
{
    OnCleanUp();
}

CGoblin::~CGoblin()
{
}

void CGoblin::OnCleanUp()
{
    m_nShouHuAct = 0;
    m_nShouHuExp = 0;
    m_nLastPolishTime = 0;
}

void CGoblin::OnUpdate(int64_t curTick)
{
}

void CGoblin::OnDaySwitch(int32_t nDiffDays)
{
}

void CGoblin::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_nShouHuAct = dbData->m_GoblinData.m_nShouHuAct;
    m_nShouHuExp = dbData->m_GoblinData.m_nShouHuExp;
    m_nLastPolishTime = dbData->m_GoblinData.m_nLastPolishTime;
}

void CGoblin::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_GoblinData.m_nShouHuAct = m_nShouHuAct;
    dbData->m_GoblinData.m_nShouHuExp = m_nShouHuExp;
    dbData->m_GoblinData.m_nLastPolishTime = m_nLastPolishTime;
}

void CGoblin::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(801);
    procList->push_back(802);
    procList->push_back(803);
    procList->push_back(804);
    procList->push_back(805);
    procList->push_back(806);
    procList->push_back(807);
}

int32_t CGoblin::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 801:
        return OnGoblinRefine(inPacket);
    case 802:
        return OnGetShouHuAct(inPacket);
    case 803:
        return OnShouHuRefine(inPacket);
    case 804:
        return OnEquipPolish(inPacket);
    case 805:
        return OnEquipPolishLock(inPacket);
    case 806:
        return OnEquipPolishUp(inPacket);
    case 807:
        return OnWingEquipPolish(inPacket);
    }
    return 0;
}

int32_t CGoblin::OnGoblinRefine(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    int32_t nStoneId = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const GoblinRefineCfg* pCfg = CfgData::GetGoblinRefineCfg(cfg, pItem->m_nId);
    if (!pCfg)
        return 10002;
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_EQUIP, nStoneId, 1, ITEM_CHANGE_REASON::ICR_GOBLIN_REFINE))
        return 10002;
    pItem->m_nRefineLevel += 1;
    Bag->UpdateItem(nGrid, ITEM_CHANGE_REASON::ICR_GOBLIN_REFINE);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, Player::getGateIndex(m_pPlayer), inPacket->getProc(), 0);
    return 0;
}

int32_t CGoblin::OnGetShouHuAct(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer)
        return 2;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD0);
    if (!packet)
        return 2;
    packet->writeInt32(m_nShouHuAct);
    packet->writeInt32(m_nShouHuExp);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
    return 0;
}

int32_t CGoblin::OnShouHuRefine(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const ShouHuRefineCfg* pCfg = CfgData::GetShouHuRefineCfg(cfg, pItem->m_nId, m_nShouHuAct);
    if (!pCfg)
        return 10002;
    if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < pCfg->nPrice)
        return 10002;
    Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->nPrice, CURRENCY_CHANGE_REASON::GCR_SHOUHU_REFINE, 0);
    pItem->m_nRefineLevel += 1;
    m_nShouHuExp += pCfg->nExp;
    Bag->UpdateItem(nGrid, ITEM_CHANGE_REASON::ICR_SHOUHU_REFINE);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, Player::getGateIndex(m_pPlayer), inPacket->getProc(), 0);
    return 0;
}

int32_t CGoblin::OnEquipPolish(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    int32_t nStoneId = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_EQUIP, nStoneId, 1, ITEM_CHANGE_REASON::ICR_EQUIP_POLISH))
        return 10002;
    Bag->UpdateItem(nGrid, ITEM_CHANGE_REASON::ICR_EQUIP_POLISH);
    return 0;
}

int32_t CGoblin::OnEquipPolishLock(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    int32_t nLockId = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_EQUIP, nLockId, 1, ITEM_CHANGE_REASON::ICR_EQUIP_POLISH))
        return 10002;
    return 0;
}

int32_t CGoblin::OnEquipPolishUp(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    Bag->UpdateItem(nGrid, ITEM_CHANGE_REASON::ICR_EQUIP_POLISH);
    return 0;
}

int32_t CGoblin::OnWingEquipPolish(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    int32_t nStoneId = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_EQUIP, nStoneId, 1, ITEM_CHANGE_REASON::ICR_WING_POLISH))
        return 10002;
    Bag->UpdateItem(nGrid, ITEM_CHANGE_REASON::ICR_WING_POLISH);
    return 0;
}

int32_t CGoblin::OnWingEquipPolishLock(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    int32_t nLockId = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_EQUIP, nLockId, 1, ITEM_CHANGE_REASON::ICR_WING_POLISH))
        return 10002;
    return 0;
}
