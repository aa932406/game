#include "Other/CSevenDayTask.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtCharWing.h"
#include "Character/CExtCharJueWei.h"
#include "Character/CExtEquip.h"
#include "Other/SevenTaskTable.h"
#include "Other/CFunctionOpen.h"
#include <string>

CSevenDayTask::CSevenDayTask()
    : CExtSystemBase()
{
    m_SevenDayTask.clear();
}

CSevenDayTask::~CSevenDayTask()
{
}

void CSevenDayTask::OnCleanUp()
{
}

void CSevenDayTask::OnUpdate(int64_t curTick)
{
}

void CSevenDayTask::OnDaySwitch(int32_t nDiffDays)
{
    m_OpenTime = 0;
    m_RewardState.clear();
    m_SumRewardState = 0;
}

void CSevenDayTask::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_SevenDayTask = dbData->m_SevenDayTaskDBData.m_SevenDayTask;
}

void CSevenDayTask::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_SevenDayTaskDBData.m_SevenDayTask = m_SevenDayTask;
}

void CSevenDayTask::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(719);
}

int32_t CSevenDayTask::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 719)
        return OnGetSevenDayTaskData(inPacket);
    return 2;
}

int32_t CSevenDayTask::OnGetSevenDayTaskData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D90);
    if (!packet)
        return 2;
    int32_t nSize = static_cast<int32_t>(m_SevenDayTask.size());
    packet->writeInt32(nSize);
    for (auto& kv : m_SevenDayTask)
    {
        packet->writeInt32(kv.first);
        packet->writeInt32(kv.second);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, ConnId, GateIndex, packet);
    return 0;
}

int32_t CSevenDayTask::OnAskReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    int32_t nId = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    SevenTaskTable* pTaskTable = CfgData::GetSevenTaskTable(cfg);
    const SevenTaskCfg* pCfg = pTaskTable->GetSevenTaskCfg(nId);
    if (!pCfg)
        return 10002;
    auto it = m_RewardState.find(nId);
    if (it == m_RewardState.end() || it->second > 0)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pCfg->Item, ITEM_CHANGE_REASON::IDCR_SEVEN_DAY))
        return 10002;
    it->second = 1;
    SendIcon();
    SendTaskSTate();
    return 0;
}

int32_t CSevenDayTask::OnAskSumReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    int32_t nId = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    SevenTaskTable* pTaskTable = CfgData::GetSevenTaskTable(cfg);
    const SevenTaskSumReward* pSumReward = pTaskTable->GetSevenTaskSumReward(nId);
    if (!pSumReward)
        return 10002;
    if (static_cast<int32_t>(m_RewardState.size()) < pSumReward->nFinishCount)
        return 10002;
    int32_t NewState = (1 << (nId - 1)) | m_SumRewardState;
    if (m_SumRewardState == NewState)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pSumReward->Item, ITEM_CHANGE_REASON::IDCR_SEVEN_DAY))
        return 10002;
    m_SumRewardState = NewState;
    SendIcon();
    SendTaskSTate();
    if (pSumReward->GongGaoId > 0)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(pSumReward->GongGaoId);
            MString name;
            Player::getPlayerName(m_pPlayer, &name);
            packet->writeUTF8(name);
            CharId_t Cid = Player::getCid(m_pPlayer);
            packet->writeInt64(Cid);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::worldBroadcast(gs, Player::getConnId(m_pPlayer), packet);
        }
    }
    return 0;
}

void CSevenDayTask::UpdateTaskState(int32_t nType, int32_t Values, bool NeedSend)
{
    if (m_OpenTime <= 0)
        return;
    int32_t DiffDay = Answer::DayTime::Diff24Hour(m_OpenTime) + 1;
    if (DiffDay > 7)
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    SevenTaskTable* pTaskTable = CfgData::GetSevenTaskTable(cfg);
    const auto& CfgMap = pTaskTable->GetSevenTaskCfgMap();
    for (auto& taskPair : CfgMap)
    {
        auto itState = m_RewardState.find(taskPair.first);
        if (itState == m_RewardState.end())
        {
            if (taskPair.second.nEndDay >= DiffDay &&
                taskPair.second.nType == nType &&
                taskPair.second.nCondition <= Values)
            {
                m_RewardState[taskPair.first] = 0;
            }
        }
    }
    if (NeedSend)
    {
        SendIcon();
        SendTaskSTate();
    }
}

void CSevenDayTask::SendTaskSTate()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CC8u);
    if (!packet)
        return;
    packet->writeInt32(m_OpenTime);
    packet->writeInt32(static_cast<int32_t>(m_RewardState.size()));
    for (auto& kv : m_RewardState)
    {
        packet->writeInt32(kv.first);
        packet->writeInt32(kv.second);
    }
    packet->writeInt32(m_SumRewardState);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, Player::getConnId(m_pPlayer), GateIndex, packet);
}

void CSevenDayTask::OpenSevenDayTask()
{
    if (!m_pPlayer || Player::getLevel(m_pPlayer) < 100 || m_OpenTime > 0)
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::getServerDiffDay(cfg, SERVER_TYPE::SVT_NORMAL) + 1 > 7)
        return;
    m_OpenTime = Unit::getNow(m_pPlayer);
    UpdateTaskState(1, Player::getLevel(m_pPlayer), false);
    UpdateTaskState(2, Player::getRecord(m_pPlayer, 1148), false);
    CExtCharWing* pWing = Player::GetCharWing(m_pPlayer);
    UpdateTaskState(3, CExtCharWing::GetLevel(pWing), false);
    CExtCharJueWei* pJueWei = Player::GetCharJueWei(m_pPlayer);
    UpdateTaskState(4, CExtCharJueWei::GetJueWei(pJueWei), false);
    CExtEquip* pEquip = Player::GetEquip(m_pPlayer);
    UpdateTaskState(5, CExtEquip::GetEquipAllGemLevel(pEquip), false);
    UpdateTaskState(6, CExtEquip::GetEquipAllUpPosLevel(pEquip), false);
    SendIcon();
    SendTaskSTate();
}

void CSevenDayTask::GetIcon(IconStateList* IconList)
{
    if (!m_pPlayer || m_OpenTime <= 0)
        return;
    int32_t DiffDay = Answer::DayTime::Diff24Hour(m_OpenTime) + 1;
    if (DiffDay > 7 && HaveRewardCount() <= 0)
        return;
    ShowIcon stu;
    GetIcon(&stu);
    IconList->push_back(stu);
}

void CSevenDayTask::SendIcon()
{
    if (!m_pPlayer || m_OpenTime <= 0)
        return;
    ShowIcon stu;
    GetIcon(&stu);
    Player::SendIconState(m_pPlayer, &stu);
}

int32_t CSevenDayTask::HaveRewardCount()
{
    if (!m_pPlayer)
        return 0;
    int32_t DiffDay = Answer::DayTime::Diff24Hour(m_OpenTime) + 1;
    int32_t Count = 0;
    int32_t FinishCount = 0;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    SevenTaskTable* pTaskTable = CfgData::GetSevenTaskTable(cfg);
    for (auto& kv : m_RewardState)
    {
        const SevenTaskCfg* pCfg = pTaskTable->GetSevenTaskCfg(kv.first);
        if (pCfg && pCfg->nStartDay <= DiffDay)
        {
            ++FinishCount;
            if (kv.second <= 0)
                ++Count;
        }
    }
    const auto& SumMap = pTaskTable->GetSevenTaskSumRewardMap();
    for (auto& srPair : SumMap)
    {
        if (srPair.second.nFinishCount <= FinishCount)
        {
            if ((m_SumRewardState & (1 << (srPair.first - 1))) == 0)
                ++Count;
        }
    }
    return Count;
}

void CSevenDayTask::GetIcon(ShowIcon* __return_ptr retstr)
{
    if (HaveRewardCount() > 0)
    {
        retstr->nId = 30;
        retstr->nState = 3;
    }
    else
    {
        retstr->nId = 30;
        retstr->nState = 2;
    }
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
}
