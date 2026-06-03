#include "common.h"
#include "Other/CBossKilledReward.h"
#include "Other/BossKilledRewardInfo.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtOperateLimit.h"
#include "Other/CFunctionOpen.h"
#include "Answer/StringUtility.h"

#include <sstream>

CBossKilledReward::CBossKilledReward()
    : CExtSystemBase()
{
    OnCleanUp();
}

CBossKilledReward::~CBossKilledReward()
{
}

void CBossKilledReward::OnCleanUp()
{
    m_BossKilledRewardMap.clear();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const BossKilledRewardMap* RewardMap = CfgData::GetBossKilledRewardMap(cfg);
    BossKilledRewardMap temp(*RewardMap);
    for (auto& pair : temp)
    {
        for (auto bossId : pair.second.BossList)
        {
            BossKilledInfo stu;
            stu.BossId = bossId;
            m_BossKilledRewardMap[pair.first].BossKeilled.push_back(stu);
            m_BossKilledRewardMap[pair.first].IsGetReward = 0;
        }
    }
}

void CBossKilledReward::OnUpdate(int64_t curTick)
{
}

void CBossKilledReward::OnDaySwitch(int32_t nDiffDays)
{
}

void CBossKilledReward::OnLoadFromDB(const PlayerDBData* const dbData)
{
    std::string delims("|");
    StringVector mineString;
    Answer::StringUtility::split(&mineString, &dbData->m_BossKilledReward.RewardString, &delims, 0);
    for (auto& str : mineString)
    {
        StringVector StringVt;
        std::string delim(":");
        Answer::StringUtility::split(&StringVt, &str, &delim, 0);
        if (StringVt.size() == 3)
        {
            int8_t Type = static_cast<int8_t>(atoi(StringVt[0].c_str()));
            int8_t IsGetReward = static_cast<int8_t>(atoi(StringVt[1].c_str()));
            auto itBoss = m_BossKilledRewardMap.find(Type);
            if (itBoss != m_BossKilledRewardMap.end())
            {
                itBoss->second.IsGetReward = IsGetReward;
                std::string delim2(",");
                StringVector BossIdVt;
                Answer::StringUtility::split(&BossIdVt, &StringVt[2], &delim2, 0);
                for (auto& bossIdStr : BossIdVt)
                {
                    int32_t BossId = atoi(bossIdStr.c_str());
                    itBoss->second.SetBossKilled(BossId);
                }
            }
        }
    }
}

void CBossKilledReward::OnSaveToDB(PlayerDBData* const dbData)
{
    std::ostringstream ss;
    for (auto& it : m_BossKilledRewardMap)
    {
        ss << static_cast<int32_t>(it.first) << ":" << static_cast<int32_t>(it.second.IsGetReward) << ":";
        for (auto& itBoss : it.second.BossKeilled)
        {
            if (itBoss.IsKilled > 0)
                ss << itBoss.BossId << ",";
        }
        ss << ":|";
    }
    dbData->m_BossKilledReward.RewardString = ss.str();
}

void CBossKilledReward::GetInterestsProtocol(ProcIdList* const procList)
{
    procList->push_back(545);
}

int32_t CBossKilledReward::OnGetBossKilledReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;

    int8_t type = static_cast<int8_t>(inPacket->readInt32());
    auto it = m_BossKilledRewardMap.find(type);
    if (it != m_BossKilledRewardMap.end())
    {
        if (!it->second.CanGetReward())
            return 10002;
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        const BossKilledReward* pCfg = CfgData::GetBossKilledReward(cfg, type);
        if (!pCfg)
            return 10002;
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!Bag->AddItem(&pCfg->Rewars, ITEM_CHANGE_REASON::ICR_GET_BOSS_KILLED_REWARD))
            return 10002;
        it->second.IsGetReward = 1;
        SendBossKilledInfo();
        SendBossKilledIcon();
        int8_t connId = Player::getConnId(m_pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            packet->writeInt32(450);
            std::string val;
            Player::getName(m_pPlayer, &val);
            packet->writeUTF8(&val);
            packet->writeInt64(Player::getCid(m_pPlayer));
            packet->writeInt8(type);
            uint32_t wOffset = packet->getWOffset();
            packet->setSize(wOffset);
            int8_t connId2 = Player::getConnId(m_pPlayer);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, connId2, packet);
        }
    }
    return 10002;
}

void CBossKilledReward::AddBossKilled(int32_t BossId)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (!CFunctionOpen::IsOpened(PlayerFunctionOpen, 190))
        return;
    for (auto& pair : m_BossKilledRewardMap)
    {
        if (pair.second.SetBossKilled(BossId))
        {
            SendBossKilledInfo();
            SendBossKilledIcon();
        }
    }
}

void CBossKilledReward::SendBossKilledInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x283E);
    if (!packet)
        return;
    packet->writeInt32(static_cast<int32_t>(m_BossKilledRewardMap.size()));
    for (auto& pair : m_BossKilledRewardMap)
    {
        packet->writeInt8(pair.first);
        packet->writeInt8(pair.second.IsGetReward);
        int32_t oldWoffset = packet->getWOffset();
        int32_t count = 0;
        packet->writeInt32(0);
        for (auto& bossInfo : pair.second.BossKeilled)
        {
            if (bossInfo.IsKilled > 0)
            {
                packet->writeInt32(bossInfo.BossId);
                ++count;
            }
        }
        int32_t newWoffset = packet->getWOffset();
        packet->setWOffset(oldWoffset);
        packet->writeInt32(count);
        packet->setWOffset(newWoffset);
    }
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

void CBossKilledReward::GetBossKilledIcon(IconStateList* const IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 190))
    {
        ShowIcon icon;
        GetBossKilledIcon(&icon);
        IconList->push_back(icon);
    }
}

void CBossKilledReward::SendBossKilledIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 190))
    {
        ShowIcon icon;
        GetBossKilledIcon(&icon);
        Player::SendIconState(m_pPlayer, &icon);
    }
}

bool CBossKilledReward::IsAllRewardGet()
{
    for (auto& pair : m_BossKilledRewardMap)
    {
        if (pair.second.IsGetReward <= 0)
            return false;
    }
    return true;
}

int32_t CBossKilledReward::CanGetRewardCount()
{
    int32_t Count = 0;
    for (auto& pair : m_BossKilledRewardMap)
    {
        if (pair.second.CanGetReward())
            ++Count;
    }
    return Count;
}

void CBossKilledReward::GetBossKilledIcon(ShowIcon* __return_ptr retstr)
{
    retstr->nId = 0;
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
    if (!m_pPlayer)
        return;
    retstr->nId = 114;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 190) && !IsAllRewardGet())
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nLeftTime = -1;
    retstr->nIconRight = CanGetRewardCount();
    if (retstr->nIconRight > 0)
        retstr->nEffects = 1;
}
