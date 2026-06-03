#include "common.h"
#include "Other/CTestServerReward.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Other/CFunctionOpen.h"
#include <string>

CTestServerReward::CTestServerReward()
    : CExtSystemBase()
{
    m_RewardData.clear();
}

CTestServerReward::~CTestServerReward()
{
}

void CTestServerReward::OnCleanUp()
{
}

void CTestServerReward::OnUpdate(int64_t curTick)
{
}

void CTestServerReward::OnDaySwitch(int32_t nDiffDays)
{
}

void CTestServerReward::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_RewardData = dbData->m_TestServerRewardDBData.m_RewardData;
}

void CTestServerReward::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_TestServerRewardDBData.m_RewardData = m_RewardData;
}

void CTestServerReward::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(721);
    procList->push_back(722);
}

int32_t CTestServerReward::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 721)
        return OnGetData(inPacket);
    if (nProcId == 722)
        return OnClaimReward(inPacket);
    return 2;
}

int32_t CTestServerReward::OnGetData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D92);
    if (!packet)
        return 2;
    int32_t nSize = static_cast<int32_t>(m_RewardData.size());
    packet->writeInt32(nSize);
    for (auto& kv : m_RewardData)
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

int32_t CTestServerReward::OnClaimReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nRewardId = inPacket->readInt32();
    if (m_RewardData.find(nRewardId) != m_RewardData.end())
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgTestServerRewardTable* pTable = CfgData::GetTestServerRewardTable(cfg);
    const CfgTestServerReward* pCfg = CfgTestServerRewardTable::GetRow(pTable, nRewardId);
    if (!pCfg)
        return 2;
    int32_t nLevelLimit = pCfg->m_LevelLimit;
    if (nLevelLimit > 0 && Player::getLevel(m_pPlayer) < nLevelLimit)
        return 2;
    int32_t nRewardGroup = pCfg->m_RewardGroup;
    if (nRewardGroup > 0)
    {
        const CfgRewardTable* pRewardTable = CfgData::GetRewardTable(cfg);
        MemChrBagVector rewardItems;
        CfgRewardTable::GetRewardById(pRewardTable, &rewardItems, nRewardGroup);
        CExtCharBag* pBag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::AddItemsAndMingGe(pBag, &rewardItems, ITEM_CHANGE_REASON::ICR_TESTSERVER_REWARD))
            return 2;
    }
    m_RewardData[nRewardId] = 1;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, ConnId, GateIndex, Proc, 0);
    return 0;
}

int32_t CTestServerReward::OnGetTestServerReward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 2;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (!CFunctionOpen::IsOpened(pFuncOpen, 192))
        return 2;
    int32_t nId = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    TestServerReward* pCfg = CfgData::GetTestServerReward(cfg, nId);
    if (!pCfg)
        return 2;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 1915);
    int32_t NewRecord = OldRecord | (1 << (nId - 1));
    if (NewRecord == OldRecord)
        return 10002;
    int32_t nType = pCfg->nType;
    if (nType == 1)
    {
        if (Player::getRecord(m_pPlayer, 1011) < pCfg->nParm)
            return 10002;
    }
    else if (nType == 2)
    {
        if (Player::getLevel(m_pPlayer) < pCfg->nParm)
            return 10002;
    }
    else
    {
        return 2;
    }
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pCfg->vItems, ITEM_CHANGE_REASON::ICR_TEST_SERVER))
        return 2;
    Player::updateRecord(m_pPlayer, 1915, NewRecord);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, ConnId, GateIndex, Proc, nId);
    if (pCfg->nGongId > 0)
    {
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(pCfg->nGongId);
            CharId_t Cid = Player::getCid(m_pPlayer);
            packet->writeInt64(Cid);
            MString name;
            Player::getPlayerName(m_pPlayer, &name);
            packet->writeUTF8(name);
            packet->writeInt32(nId);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::worldBroadcast(gs, 0, packet);
        }
    }
    SendTestServerIcon();
    return 0;
}

void CTestServerReward::GetTestServerIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 192))
    {
        ShowIcon stu;
        GetTestServerIconStu(&stu);
        IconList->push_back(stu);
    }
}

void CTestServerReward::SendTestServerIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 192))
    {
        ShowIcon stu;
        GetTestServerIconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}

void CTestServerReward::GetTestServerIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    retstr->nId = 116;
    if (IsAllGetReward())
        retstr->nState = 4;
    else
        retstr->nState = 2;
    retstr->nLeftTime = -1;
    retstr->nIconRight = CanActivationCount();
}

int32_t CTestServerReward::CanActivationCount()
{
    if (!m_pPlayer)
        return 0;
    int32_t Count = 0;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 1915);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    auto RewardMap = CfgData::GetTestServerRewardMap(cfg);
    for (auto& rewardPair : RewardMap)
    {
        int32_t NewRecord = OldRecord | (1 << (rewardPair.first - 1));
        if (NewRecord != OldRecord)
        {
            int32_t nType = rewardPair.second.nType;
            if (nType == 1)
            {
                if (Player::getRecord(m_pPlayer, 1011) >= rewardPair.second.nParm)
                    ++Count;
            }
            else if (nType == 2)
            {
                if (Player::getLevel(m_pPlayer) >= rewardPair.second.nParm)
                    ++Count;
            }
        }
    }
    return Count;
}

bool CTestServerReward::IsAllGetReward()
{
    if (!m_pPlayer)
        return true;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 1915);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    auto RewardMap = CfgData::GetTestServerRewardMap(cfg);
    for (auto& rewardPair : RewardMap)
    {
        int32_t NewRecord = OldRecord | (1 << (rewardPair.first - 1));
        if (NewRecord != OldRecord)
            return false;
    }
    return true;
}
