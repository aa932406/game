#include "common.h"
#include "Other/CMingGeExt.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"

CMingGeExt::CMingGeExt()
    : CExtSystemBase()
{
    OnCleanUp();
}

CMingGeExt::~CMingGeExt()
{
}

void CMingGeExt::OnCleanUp()
{
    m_nMingGeLevel = 0;
    m_nMingGeExp = 0;
    m_nLeiMingLevel = 0;
    m_nLeiMingExp = 0;
    m_nEquipRefineLevel = 0;
    m_EquipSuitRefineMap.clear();
}

void CMingGeExt::OnUpdate(int64_t curTick)
{
}

void CMingGeExt::OnDaySwitch(int32_t nDiffDays)
{
}

void CMingGeExt::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_nMingGeLevel = dbData->m_MingGeExtData.m_nMingGeLevel;
    m_nMingGeExp = dbData->m_MingGeExtData.m_nMingGeExp;
    m_nLeiMingLevel = dbData->m_MingGeExtData.m_nLeiMingLevel;
    m_nLeiMingExp = dbData->m_MingGeExtData.m_nLeiMingExp;
    m_nEquipRefineLevel = dbData->m_MingGeExtData.m_nEquipRefineLevel;
    m_EquipSuitRefineMap = dbData->m_MingGeExtData.m_EquipSuitRefineMap;
}

void CMingGeExt::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_MingGeExtData.m_nMingGeLevel = m_nMingGeLevel;
    dbData->m_MingGeExtData.m_nMingGeExp = m_nMingGeExp;
    dbData->m_MingGeExtData.m_nLeiMingLevel = m_nLeiMingLevel;
    dbData->m_MingGeExtData.m_nLeiMingExp = m_nLeiMingExp;
    dbData->m_MingGeExtData.m_nEquipRefineLevel = m_nEquipRefineLevel;
    dbData->m_MingGeExtData.m_EquipSuitRefineMap = m_EquipSuitRefineMap;
}

void CMingGeExt::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(820);
    procList->push_back(821);
    procList->push_back(822);
    procList->push_back(823);
}

int32_t CMingGeExt::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 820:
        return OnMingGeRefine(inPacket);
    case 821:
        return OnLeiMingRefine(inPacket);
    case 822:
        return OnEquipRefine(inPacket);
    case 823:
        return OnEquipSuitRefine(inPacket);
    }
    return 0;
}

int32_t CMingGeExt::OnMingGeRefine(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const MingGeCfg* pCfg = CfgData::GetMingGeCfg(cfg, m_nMingGeLevel + 1);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_ITEM, pCfg->nNeedItem, pCfg->nNeedCount, ITEM_CHANGE_REASON::ICR_MING_GE))
        return 10002;
    m_nMingGeExp += pCfg->nExp;
    while (true)
    {
        const MingGeCfg* pNext = CfgData::GetMingGeCfg(cfg, m_nMingGeLevel + 1);
        if (!pNext)
            break;
        if (m_nMingGeExp < pNext->nNeedExp)
            break;
        m_nMingGeExp -= pNext->nNeedExp;
        ++m_nMingGeLevel;
    }
    SendMingGeInfo();
    return 0;
}

int32_t CMingGeExt::OnLeiMingRefine(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const LeiMingCfg* pCfg = CfgData::GetLeiMingCfg(cfg, m_nLeiMingLevel + 1);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_ITEM, pCfg->nNeedItem, pCfg->nNeedCount, ITEM_CHANGE_REASON::ICR_LEI_MING))
        return 10002;
    m_nLeiMingExp += pCfg->nExp;
    while (true)
    {
        const LeiMingCfg* pNext = CfgData::GetLeiMingCfg(cfg, m_nLeiMingLevel + 1);
        if (!pNext)
            break;
        if (m_nLeiMingExp < pNext->nNeedExp)
            break;
        m_nLeiMingExp -= pNext->nNeedExp;
        ++m_nLeiMingLevel;
    }
    SendLeiMingInfo();
    return 0;
}

int32_t CMingGeExt::OnEquipRefine(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nGrid = inPacket->readInt32();
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    ItemInfo* pItem = Bag->GetItem(nGrid);
    if (!pItem)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const EquipRefineCfg* pCfg = CfgData::GetEquipRefineCfg(cfg, pItem->m_nId);
    if (!pCfg)
        return 10002;
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_EQUIP, pCfg->nNeedStone, 1, ITEM_CHANGE_REASON::ICR_EQUIP_REFINE))
        return 10002;
    pItem->m_nRefineLevel += 1;
    Bag->UpdateItem(nGrid, ITEM_CHANGE_REASON::ICR_EQUIP_REFINE);
    return 0;
}

int32_t CMingGeExt::OnEquipSuitRefine(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nSuitId = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const EquipSuitRefineCfg* pCfg = CfgData::GetEquipSuitRefineCfg(cfg, nSuitId);
    if (!pCfg)
        return 10002;
    int32_t nCurLevel = m_EquipSuitRefineMap[nSuitId];
    const EquipSuitRefineLevelCfg* pLevelCfg = CfgData::GetEquipSuitRefineLevelCfg(cfg, nSuitId, nCurLevel + 1);
    if (!pLevelCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_ITEM, pLevelCfg->nNeedItem, pLevelCfg->nNeedCount, ITEM_CHANGE_REASON::ICR_EQUIP_SUIT_REFINE))
        return 10002;
    m_EquipSuitRefineMap[nSuitId] = nCurLevel + 1;
    SendEquipRefineInfo();
    return 0;
}

void CMingGeExt::SendMingGeInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD4);
    if (!packet)
        return;
    packet->writeInt32(m_nMingGeLevel);
    packet->writeInt32(m_nMingGeExp);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CMingGeExt::SendLeiMingInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD5);
    if (!packet)
        return;
    packet->writeInt32(m_nLeiMingLevel);
    packet->writeInt32(m_nLeiMingExp);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CMingGeExt::SendEquipRefineInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CF1);
    if (!packet)
        return;
    packet->writeInt32(m_nEquipRefineLevel);
    packet->writeInt32(static_cast<int32_t>(m_EquipSuitRefineMap.size()));
    for (auto& pair : m_EquipSuitRefineMap)
    {
        packet->writeInt32(pair.first);
        packet->writeInt32(pair.second);
    }
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}
