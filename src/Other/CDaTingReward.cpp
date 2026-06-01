#include "Other/CDaTingReward.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"

CDaTingReward::CDaTingReward()
    : CExtSystemBase()
{
    OnCleanUp();
}

CDaTingReward::~CDaTingReward()
{
}

void CDaTingReward::OnCleanUp()
{
    m_nDaTingLevel = 0;
    m_nDaTingExp = 0;
    m_DaTingRewardFlag.clear();
    m_nPlatformVipLevel = 0;
    m_nPlatformVipExp = 0;
    m_PlatformVipActRewardFlag.clear();
}

void CDaTingReward::OnUpdate(int64_t curTick)
{
}

void CDaTingReward::OnDaySwitch(int32_t nDiffDays)
{
}

void CDaTingReward::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_nDaTingLevel = dbData->m_DaTingRewardData.m_nDaTingLevel;
    m_nDaTingExp = dbData->m_DaTingRewardData.m_nDaTingExp;
    m_DaTingRewardFlag = dbData->m_DaTingRewardData.m_DaTingRewardFlag;
    m_nPlatformVipLevel = dbData->m_DaTingRewardData.m_nPlatformVipLevel;
    m_nPlatformVipExp = dbData->m_DaTingRewardData.m_nPlatformVipExp;
    m_PlatformVipActRewardFlag = dbData->m_DaTingRewardData.m_PlatformVipActRewardFlag;
}

void CDaTingReward::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_DaTingRewardData.m_nDaTingLevel = m_nDaTingLevel;
    dbData->m_DaTingRewardData.m_nDaTingExp = m_nDaTingExp;
    dbData->m_DaTingRewardData.m_DaTingRewardFlag = m_DaTingRewardFlag;
    dbData->m_DaTingRewardData.m_nPlatformVipLevel = m_nPlatformVipLevel;
    dbData->m_DaTingRewardData.m_nPlatformVipExp = m_nPlatformVipExp;
    dbData->m_DaTingRewardData.m_PlatformVipActRewardFlag = m_PlatformVipActRewardFlag;
}

void CDaTingReward::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(580);
    procList->push_back(581);
    procList->push_back(582);
    procList->push_back(583);
    procList->push_back(584);
}

int32_t CDaTingReward::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 580:
        return OnDaTingInfo(inPacket);
    case 581:
        return OnDaTingReward(inPacket);
    case 582:
        return OnDaTingBuy(inPacket);
    case 583:
        return OnPlatformVipLevelUp(inPacket);
    case 584:
        return OnPlatformVipActReward(inPacket);
    }
    return 0;
}

int32_t CDaTingReward::OnDaTingInfo(Answer::NetPacket* inPacket)
{
    SendDaTingInfo();
    return 0;
}

int32_t CDaTingReward::OnDaTingReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nLevel = inPacket->readInt32();
    auto it = m_DaTingRewardFlag.find(nLevel);
    if (it != m_DaTingRewardFlag.end() && it->second > 0)
        return 10002;
    if (m_nDaTingLevel < nLevel)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const DaTingLevelCfg* pCfg = CfgData::GetDaTingLevelCfg(cfg, nLevel);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_DA_TING_REWARD))
        return 10002;
    m_DaTingRewardFlag[nLevel] = 1;
    SendDaTingInfo();
    return 0;
}

int32_t CDaTingReward::OnDaTingBuy(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nBuyLevel = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const DaTingLevelCfg* pCfg = CfgData::GetDaTingLevelCfg(cfg, nBuyLevel);
    if (!pCfg)
        return 10002;
    if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < pCfg->nBuyPrice)
        return 10002;
    Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->nBuyPrice, CURRENCY_CHANGE_REASON::GCR_DA_TING_BUY, 0);
    m_nDaTingExp += pCfg->nExp;
    while (true)
    {
        const DaTingLevelCfg* pNext = CfgData::GetDaTingLevelCfg(cfg, m_nDaTingLevel + 1);
        if (!pNext)
            break;
        if (m_nDaTingExp < pNext->nNeedExp)
            break;
        m_nDaTingExp -= pNext->nNeedExp;
        ++m_nDaTingLevel;
    }
    SendDaTingInfo();
    return 0;
}

int32_t CDaTingReward::OnPlatformVipLevelUp(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nTargetLevel = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const PlatformVipCfg* pCfg = CfgData::GetPlatformVipCfg(cfg, nTargetLevel);
    if (!pCfg)
        return 10002;
    if (m_nPlatformVipExp < pCfg->nNeedExp)
        return 10002;
    m_nPlatformVipExp -= pCfg->nNeedExp;
    m_nPlatformVipLevel = nTargetLevel;
    SendPlatformVipInfo();
    return 0;
}

int32_t CDaTingReward::OnPlatformVipActReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nActId = inPacket->readInt32();
    auto it = m_PlatformVipActRewardFlag.find(nActId);
    if (it != m_PlatformVipActRewardFlag.end() && it->second > 0)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const PlatformVipActCfg* pCfg = CfgData::GetPlatformVipActCfg(cfg, nActId);
    if (!pCfg)
        return 10002;
    if (m_nPlatformVipLevel < pCfg->nNeedLevel)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_PLATFORM_VIP_REWARD))
        return 10002;
    m_PlatformVipActRewardFlag[nActId] = 1;
    SendPlatformVipInfo();
    return 0;
}

void CDaTingReward::SendDaTingInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CE9);
    if (!packet)
        return;
    packet->writeInt32(m_nDaTingLevel);
    packet->writeInt32(m_nDaTingExp);
    packet->writeInt32(static_cast<int32_t>(m_DaTingRewardFlag.size()));
    for (auto& pair : m_DaTingRewardFlag)
    {
        packet->writeInt32(pair.first);
        packet->writeInt32(pair.second);
    }
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CDaTingReward::SendPlatformVipInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CEA);
    if (!packet)
        return;
    packet->writeInt32(m_nPlatformVipLevel);
    packet->writeInt32(m_nPlatformVipExp);
    packet->writeInt32(static_cast<int32_t>(m_PlatformVipActRewardFlag.size()));
    for (auto& pair : m_PlatformVipActRewardFlag)
    {
        packet->writeInt32(pair.first);
        packet->writeInt32(pair.second);
    }
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CDaTingReward::AddPlatformVipExp(int32_t nExp)
{
    m_nPlatformVipExp += nExp;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    while (true)
    {
        const PlatformVipCfg* pNext = CfgData::GetPlatformVipCfg(cfg, m_nPlatformVipLevel + 1);
        if (!pNext)
            break;
        if (m_nPlatformVipExp < pNext->nNeedExp)
            break;
        m_nPlatformVipExp -= pNext->nNeedExp;
        ++m_nPlatformVipLevel;
    }
    SendPlatformVipInfo();
}
