#include "common.h"
#include "Other/CHuoYueDu.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtEquip.h"
#include "Other/CFunctionOpen.h"
#include "Other/CFestivalDoubleEleven.h"

CHuoYueDu::CHuoYueDu()
    : CExtSystemBase()
{
    OnCleanUp();
}

CHuoYueDu::~CHuoYueDu()
{
}

void CHuoYueDu::OnCleanUp()
{
    m_HuoYueDuRecord.clear();
}

void CHuoYueDu::OnUpdate(int64_t curTick)
{
}

void CHuoYueDu::OnDaySwitch(int32_t nDiffDays)
{
    m_HuoYueDuRecord.clear();
    if (!m_pPlayer)
        return;
    if (m_pPlayer->getLevel() >= Player::GetMaxLevel(m_pPlayer))
        AddHuoYueDuRecord(6, 0, true);
    if (Player::GetEquip(m_pPlayer)->IsAllPosTop())
        AddHuoYueDuRecord(10, 0, true);
    if (Player::IsMiniClient(m_pPlayer))
        AddHuoYueDuRecord(7, 0, false);
}

void CHuoYueDu::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_HuoYueDuRecord = dbData->m_HuoYueDuData.m_HuoYueDuRecord;
}

void CHuoYueDu::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_HuoYueDuData.m_HuoYueDuRecord = m_HuoYueDuRecord;
}

void CHuoYueDu::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(500);
    procList->push_back(501);
    procList->push_back(502);
}

int32_t CHuoYueDu::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 0x1F5:
        return OnGetHuoYueDuReward(inPacket);
    case 0x1F6:
        return OnSec(inPacket);
    case 0x1F4:
        return OnAskHuoYueDuInfo(inPacket);
    }
    return 0;
}

int32_t CHuoYueDu::OnAskHuoYueDuInfo(Answer::NetPacket* inPacket)
{
    SendHuoYueDuInfo();
    return 0;
}

int32_t CHuoYueDu::OnGetHuoYueDuReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t Index = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    CfgHuoYueDuReward* pReward = CfgData::GetHuoYueDuReward(cfg, Index);
    if (!pReward)
        return 2;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 2012);
    int NewRecord = OldRecord | (1 << (Index - 1));
    if (OldRecord == NewRecord)
        return 2;
    if (pReward->NeedHuoYueDu > CalculateHuoYueDu())
        return 2;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pReward->Items, ITEM_CHANGE_REASON::ICR_HUO_YUE_DU))
        return 2;
    Player::updateRecord(m_pPlayer, 2012, NewRecord);
    SendtHuoYueDuIcon();
    SendHuoYueDuInfo();
    return 0;
}

int32_t CHuoYueDu::OnSec(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t Index = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    CfgHuoYueDu* pHuoYueDu = CfgData::GetHuoYueDuCfg(cfg, Index);
    if (!pHuoYueDu)
        return 2;
    int32_t AddHuoYueDu = pHuoYueDu->AddHuoYueDu;
    int32_t nAdd = AddHuoYueDu;
    if (Answer::Singleton<CFestivalDoubleEleven>::instance()->isOpen())
        nAdd = AddHuoYueDu;
    auto it = m_HuoYueDuRecord.find(Index);
    if (it != m_HuoYueDuRecord.end())
    {
        it->second.nCount += 1;
        if (it->second.nCount <= pHuoYueDu->FinishTimes)
            AddHuoYueDu(nAdd);
    }
    else
    {
        HuoYueDuRecord record;
        record.nCount = 1;
        record.IsGetReward = 0;
        m_HuoYueDuRecord[Index] = record;
        AddHuoYueDu(nAdd);
    }
    SendHuoYueDuInfo();
    return 0;
}

void CHuoYueDu::AddKiaFuHuoYueDu(int32_t Value)
{
    if (!m_pPlayer)
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::getServerDiffDay(cfg, SERVER_TYPE::SVT_NORMAL) > 9)
        return;
    AddHuoYueDuRecord(9, static_cast<int8_t>(Value), false);
}

void CHuoYueDu::AddHuoYueDuRecord(int8_t Type, int32_t Effect, bool Complete)
{
    if (!m_pPlayer)
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t Index = Type;
    auto it = m_HuoYueDuRecord.find(Index);
    if (it != m_HuoYueDuRecord.end())
    {
        ++it->second.nCount;
    }
    else
    {
        HuoYueDuRecord record;
        record.nCount = 1;
        record.IsGetReward = 0;
        m_HuoYueDuRecord[Index] = record;
    }
    CfgHuoYueDu* pHuoYueDu = CfgData::GetHuoYueDuCfg(cfg, Index);
    if (pHuoYueDu && m_HuoYueDuRecord[Index].nCount >= pHuoYueDu->FinishTimes && Complete)
    {
        m_HuoYueDuRecord[Index].IsGetReward = 1;
        int32_t AddHuoYueDu = pHuoYueDu->AddHuoYueDu;
        if (Answer::Singleton<CFestivalDoubleEleven>::instance()->isOpen())
            AddHuoYueDu = static_cast<int32_t>(AddHuoYueDu * 1.5f);
        Player::AddCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_HUOYUEDU, AddHuoYueDu, CURRENCY_CHANGE_REASON::SCR_HUO_YUE_DU, 0);
    }
    SendtHuoYueDuIcon();
}

void CHuoYueDu::SendHuoYueDuInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CFA);
    if (!packet)
        return;
    int32_t Record = Player::getRecord(m_pPlayer, 2012);
    packet->writeInt32(Record);
    packet->writeInt32(static_cast<int32_t>(m_HuoYueDuRecord.size()));
    for (auto& pair : m_HuoYueDuRecord)
    {
        packet->writeInt32(pair.first);
        packet->writeInt32(pair.second.nCount);
        packet->writeInt8(pair.second.IsGetReward);
    }
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

int32_t CHuoYueDu::CalculateHuoYueDu()
{
    int32_t Count = 0;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    for (auto& pair : m_HuoYueDuRecord)
    {
        CfgHuoYueDu* pHuoYueDu = CfgData::GetHuoYueDuCfg(cfg, pair.first);
        if (pHuoYueDu && pair.second.IsGetReward > 0)
            Count += pHuoYueDu->AddHuoYueDu;
    }
    return Count;
}

int32_t CHuoYueDu::RewardCount()
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t Count = 0;
    int32_t TotalHuoYueDu = CalculateHuoYueDu();
    int32_t OldRecord = Player::getRecord(m_pPlayer, 2012);
    const HuoYueDuRewardTable* RewardTable = CfgData::GetHuoYueDuRewardTable(cfg);
    HuoYueDuRewardTable stu(*RewardTable);
    for (auto& pair : stu)
    {
        int32_t NewRecord = OldRecord | (1 << (static_cast<uint8_t>(pair.first) - 1));
        if (NewRecord != OldRecord && pair.second.NeedHuoYueDu <= TotalHuoYueDu)
            ++Count;
    }
    return Count;
}

void CHuoYueDu::GetHuoYueDuIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 197))
    {
        ShowIcon icon;
        GettHuoYueDuIcon(&icon);
        IconList->push_back(icon);
    }
}

void CHuoYueDu::SendtHuoYueDuIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 197))
    {
        ShowIcon icon;
        GettHuoYueDuIcon(&icon);
        Player::SendIconState(m_pPlayer, &icon);
    }
}

void CHuoYueDu::GettHuoYueDuIcon(ShowIcon* __return_ptr retstr)
{
    retstr->nId = 0;
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
    if (!m_pPlayer)
        return;
    retstr->nId = 125;
    retstr->nState = CalculateHuoYueDu() > 0 ? 2 : 4;
    retstr->nLeftTime = -1;
    retstr->nIconRight = RewardCount();
    if (retstr->nIconRight > 0)
        retstr->nEffects = 1;
}
