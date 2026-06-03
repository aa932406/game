#include "common.h"
#include "Other/CKunExt.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"

CKunExt::CKunExt()
    : CExtSystemBase()
{
    OnCleanUp();
}

CKunExt::~CKunExt()
{
}

void CKunExt::OnCleanUp()
{
    m_nKunLingLevel = 0;
    m_nKunLingExp = 0;
    m_nLingZhuLevel = 0;
    m_nLingZhuExp = 0;
    m_nDanTianLevel = 0;
    m_nDanTianExp = 0;
}

void CKunExt::OnUpdate(int64_t curTick)
{
}

void CKunExt::OnDaySwitch(int32_t nDiffDays)
{
}

void CKunExt::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_nKunLingLevel = dbData->m_KunExtData.m_nKunLingLevel;
    m_nKunLingExp = dbData->m_KunExtData.m_nKunLingExp;
    m_nLingZhuLevel = dbData->m_KunExtData.m_nLingZhuLevel;
    m_nLingZhuExp = dbData->m_KunExtData.m_nLingZhuExp;
    m_nDanTianLevel = dbData->m_KunExtData.m_nDanTianLevel;
    m_nDanTianExp = dbData->m_KunExtData.m_nDanTianExp;
}

void CKunExt::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_KunExtData.m_nKunLingLevel = m_nKunLingLevel;
    dbData->m_KunExtData.m_nKunLingExp = m_nKunLingExp;
    dbData->m_KunExtData.m_nLingZhuLevel = m_nLingZhuLevel;
    dbData->m_KunExtData.m_nLingZhuExp = m_nLingZhuExp;
    dbData->m_KunExtData.m_nDanTianLevel = m_nDanTianLevel;
    dbData->m_KunExtData.m_nDanTianExp = m_nDanTianExp;
}

void CKunExt::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(810);
    procList->push_back(811);
    procList->push_back(812);
    procList->push_back(813);
    procList->push_back(814);
    procList->push_back(815);
}

int32_t CKunExt::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 810:
        return OnKunLing(inPacket);
    case 811:
        return OnLingZhu(inPacket);
    case 812:
        return OnDanTian(inPacket);
    case 813:
        return OnKunLingUpgrade(inPacket);
    case 814:
        return OnLingZhuUpgrade(inPacket);
    case 815:
        return OnDanTianUpgrade(inPacket);
    }
    return 0;
}

int32_t CKunExt::OnKunLing(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const KunLingCfg* pCfg = CfgData::GetKunLingCfg(cfg, m_nKunLingLevel + 1);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_ITEM, pCfg->nNeedItem, pCfg->nNeedCount, ITEM_CHANGE_REASON::ICR_KUN_LING))
        return 10002;
    m_nKunLingExp += pCfg->nExp;
    while (true)
    {
        const KunLingCfg* pNext = CfgData::GetKunLingCfg(cfg, m_nKunLingLevel + 1);
        if (!pNext)
            break;
        if (m_nKunLingExp < pNext->nNeedExp)
            break;
        m_nKunLingExp -= pNext->nNeedExp;
        ++m_nKunLingLevel;
    }
    SendKunLingInfo();
    return 0;
}

int32_t CKunExt::OnLingZhu(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const LingZhuCfg* pCfg = CfgData::GetLingZhuCfg(cfg, m_nLingZhuLevel + 1);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_ITEM, pCfg->nNeedItem, pCfg->nNeedCount, ITEM_CHANGE_REASON::ICR_LING_ZHU))
        return 10002;
    m_nLingZhuExp += pCfg->nExp;
    while (true)
    {
        const LingZhuCfg* pNext = CfgData::GetLingZhuCfg(cfg, m_nLingZhuLevel + 1);
        if (!pNext)
            break;
        if (m_nLingZhuExp < pNext->nNeedExp)
            break;
        m_nLingZhuExp -= pNext->nNeedExp;
        ++m_nLingZhuLevel;
    }
    SendLingZhuInfo();
    return 0;
}

int32_t CKunExt::OnDanTian(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const DanTianCfg* pCfg = CfgData::GetDanTianCfg(cfg, m_nDanTianLevel + 1);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->RemoveItemByClassId(ITEM_CLASS::IC_ITEM, pCfg->nNeedItem, pCfg->nNeedCount, ITEM_CHANGE_REASON::ICR_DAN_TIAN))
        return 10002;
    m_nDanTianExp += pCfg->nExp;
    while (true)
    {
        const DanTianCfg* pNext = CfgData::GetDanTianCfg(cfg, m_nDanTianLevel + 1);
        if (!pNext)
            break;
        if (m_nDanTianExp < pNext->nNeedExp)
            break;
        m_nDanTianExp -= pNext->nNeedExp;
        ++m_nDanTianLevel;
    }
    SendDanTianInfo();
    return 0;
}

int32_t CKunExt::OnKunLingUpgrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nTargetLevel = inPacket->readInt32();
    const KunLingCfg* pCfg = CfgData::GetKunLingCfg(cfg, nTargetLevel);
    if (!pCfg)
        return 10002;
    if (m_nKunLingExp < pCfg->nNeedExp)
        return 10002;
    m_nKunLingExp -= pCfg->nNeedExp;
    m_nKunLingLevel = nTargetLevel;
    SendKunLingInfo();
    return 0;
}

int32_t CKunExt::OnLingZhuUpgrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nTargetLevel = inPacket->readInt32();
    const LingZhuCfg* pCfg = CfgData::GetLingZhuCfg(cfg, nTargetLevel);
    if (!pCfg)
        return 10002;
    if (m_nLingZhuExp < pCfg->nNeedExp)
        return 10002;
    m_nLingZhuExp -= pCfg->nNeedExp;
    m_nLingZhuLevel = nTargetLevel;
    SendLingZhuInfo();
    return 0;
}

int32_t CKunExt::OnDanTianUpgrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nTargetLevel = inPacket->readInt32();
    const DanTianCfg* pCfg = CfgData::GetDanTianCfg(cfg, nTargetLevel);
    if (!pCfg)
        return 10002;
    if (m_nDanTianExp < pCfg->nNeedExp)
        return 10002;
    m_nDanTianExp -= pCfg->nNeedExp;
    m_nDanTianLevel = nTargetLevel;
    SendDanTianInfo();
    return 0;
}

void CKunExt::SendKunExtInfo()
{
    SendKunLingInfo();
    SendLingZhuInfo();
    SendDanTianInfo();
}

void CKunExt::SendKunLingInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD1);
    if (!packet)
        return;
    packet->writeInt32(m_nKunLingLevel);
    packet->writeInt32(m_nKunLingExp);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CKunExt::SendLingZhuInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD2);
    if (!packet)
        return;
    packet->writeInt32(m_nLingZhuLevel);
    packet->writeInt32(m_nLingZhuExp);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CKunExt::SendDanTianInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD3);
    if (!packet)
        return;
    packet->writeInt32(m_nDanTianLevel);
    packet->writeInt32(m_nDanTianExp);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}
