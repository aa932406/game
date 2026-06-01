#include "Other/CMoneyRewardTask.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"

CMoneyRewardTask::CMoneyRewardTask()
    : CExtSystemBase()
{
    OnCleanUp();
}

CMoneyRewardTask::~CMoneyRewardTask()
{
}

void CMoneyRewardTask::OnCleanUp()
{
    m_TaskProgress.clear();
    m_TaskRewardFlag.clear();
    m_FinishedTaskList.clear();
    m_nPdbfLevel = 0;
    m_nPdbfExp = 0;
    m_nEquipBackLevel = 0;
    m_nTrailerLevel = 0;
}

void CMoneyRewardTask::OnUpdate(int64_t curTick)
{
}

void CMoneyRewardTask::OnDaySwitch(int32_t nDiffDays)
{
}

void CMoneyRewardTask::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_TaskProgress = dbData->m_MoneyRewardTaskData.m_TaskProgress;
    m_TaskRewardFlag = dbData->m_MoneyRewardTaskData.m_TaskRewardFlag;
    m_FinishedTaskList = dbData->m_MoneyRewardTaskData.m_FinishedTaskList;
    m_nPdbfLevel = dbData->m_MoneyRewardTaskData.m_nPdbfLevel;
    m_nPdbfExp = dbData->m_MoneyRewardTaskData.m_nPdbfExp;
    m_nEquipBackLevel = dbData->m_MoneyRewardTaskData.m_nEquipBackLevel;
    m_nTrailerLevel = dbData->m_MoneyRewardTaskData.m_nTrailerLevel;
}

void CMoneyRewardTask::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_MoneyRewardTaskData.m_TaskProgress = m_TaskProgress;
    dbData->m_MoneyRewardTaskData.m_TaskRewardFlag = m_TaskRewardFlag;
    dbData->m_MoneyRewardTaskData.m_FinishedTaskList = m_FinishedTaskList;
    dbData->m_MoneyRewardTaskData.m_nPdbfLevel = m_nPdbfLevel;
    dbData->m_MoneyRewardTaskData.m_nPdbfExp = m_nPdbfExp;
    dbData->m_MoneyRewardTaskData.m_nEquipBackLevel = m_nEquipBackLevel;
    dbData->m_MoneyRewardTaskData.m_nTrailerLevel = m_nTrailerLevel;
}

void CMoneyRewardTask::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(590);
    procList->push_back(591);
    procList->push_back(592);
    procList->push_back(593);
    procList->push_back(594);
    procList->push_back(595);
    procList->push_back(596);
}

int32_t CMoneyRewardTask::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 590:
        return OnTaskInfo(inPacket);
    case 591:
        return OnTaskReward(inPacket);
    case 592:
        return OnPdbfInfo(inPacket);
    case 593:
        return OnEquipBackInfo(inPacket);
    case 594:
        return OnTrailerInfo(inPacket);
    case 595:
        return OnGetReward(inPacket);
    case 596:
        return OnGetActReward(inPacket);
    }
    return 0;
}

int32_t CMoneyRewardTask::OnTaskInfo(Answer::NetPacket* inPacket)
{
    SendTaskInfo();
    return 0;
}

int32_t CMoneyRewardTask::OnTaskReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nTaskId = inPacket->readInt32();
    if (!IsTaskCompleted(nTaskId))
        return 10002;
    auto it = m_TaskRewardFlag.find(nTaskId);
    if (it != m_TaskRewardFlag.end() && it->second > 0)
        return 10002;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const MoneyRewardTaskCfg* pCfg = CfgData::GetMoneyRewardTaskCfg(cfg, nTaskId);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_MONEY_REWARD_TASK))
        return 10002;
    m_TaskRewardFlag[nTaskId] = 1;
    SendTaskInfo();
    return 0;
}

int32_t CMoneyRewardTask::OnPdbfInfo(Answer::NetPacket* inPacket)
{
    SendPdbfInfo();
    return 0;
}

int32_t CMoneyRewardTask::OnEquipBackInfo(Answer::NetPacket* inPacket)
{
    SendEquipBackInfo();
    return 0;
}

int32_t CMoneyRewardTask::OnTrailerInfo(Answer::NetPacket* inPacket)
{
    SendTrailerInfo();
    return 0;
}

int32_t CMoneyRewardTask::OnGetReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nType = inPacket->readInt32();
    int32_t nLevel = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    if (nType == 0)
    {
        const PdbfLevelCfg* pCfg = CfgData::GetPdbfLevelCfg(cfg, nLevel);
        if (!pCfg)
            return 10002;
        if (m_nPdbfLevel < nLevel)
            return 10002;
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_MONEY_REWARD_TASK))
            return 10002;
    }
    else if (nType == 1)
    {
        const EquipBackLevelCfg* pCfg = CfgData::GetEquipBackLevelCfg(cfg, nLevel);
        if (!pCfg)
            return 10002;
        if (m_nEquipBackLevel < nLevel)
            return 10002;
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_MONEY_REWARD_TASK))
            return 10002;
    }
    else if (nType == 2)
    {
        const TrailerLevelCfg* pCfg = CfgData::GetTrailerLevelCfg(cfg, nLevel);
        if (!pCfg)
            return 10002;
        if (m_nTrailerLevel < nLevel)
            return 10002;
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_MONEY_REWARD_TASK))
            return 10002;
    }
    return 0;
}

int32_t CMoneyRewardTask::OnGetActReward(Answer::NetPacket* inPacket)
{
    return OnGetReward(inPacket);
}

void CMoneyRewardTask::SendTaskInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CE5);
    if (!packet)
        return;
    packet->writeInt32(static_cast<int32_t>(m_TaskProgress.size()));
    for (auto& pair : m_TaskProgress)
    {
        packet->writeInt32(pair.first);
        packet->writeInt32(pair.second);
    }
    packet->writeInt32(static_cast<int32_t>(m_TaskRewardFlag.size()));
    for (auto& pair : m_TaskRewardFlag)
    {
        packet->writeInt32(pair.first);
        packet->writeInt32(pair.second);
    }
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CMoneyRewardTask::SendPdbfInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CE6);
    if (!packet)
        return;
    packet->writeInt32(m_nPdbfLevel);
    packet->writeInt32(m_nPdbfExp);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CMoneyRewardTask::SendEquipBackInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CE7);
    if (!packet)
        return;
    packet->writeInt32(m_nEquipBackLevel);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CMoneyRewardTask::SendTrailerInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CE8);
    if (!packet)
        return;
    packet->writeInt32(m_nTrailerLevel);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    GameService::sendPacketTo(gs, connId, Player::getGateIndex(m_pPlayer), packet);
}

void CMoneyRewardTask::AddTaskProgress(int32_t nTaskId, int32_t nAdd)
{
    m_TaskProgress[nTaskId] += nAdd;
    CheckTaskComplete();
    SendTaskInfo();
}

bool CMoneyRewardTask::IsTaskCompleted(int32_t nTaskId)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const MoneyRewardTaskCfg* pCfg = CfgData::GetMoneyRewardTaskCfg(cfg, nTaskId);
    if (!pCfg)
        return false;
    auto it = m_TaskProgress.find(nTaskId);
    if (it == m_TaskProgress.end())
        return false;
    return it->second >= pCfg->nNeedCount;
}

void CMoneyRewardTask::CheckTaskComplete()
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    for (auto& pair : m_TaskProgress)
    {
        const MoneyRewardTaskCfg* pCfg = CfgData::GetMoneyRewardTaskCfg(cfg, pair.first);
        if (!pCfg)
            continue;
        if (pair.second >= pCfg->nNeedCount)
            m_FinishedTaskList.push_back(pair.first);
    }
}
