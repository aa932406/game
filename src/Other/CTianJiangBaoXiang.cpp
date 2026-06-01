#include "Other/CTianJiangBaoXiang.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "DBService.h"
#include "Player.h"
#include "Plant.h"

CTianJiangBaoXiang::CTianJiangBaoXiang(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    GradeBaoXiangState = 0;
}

CTianJiangBaoXiang::~CTianJiangBaoXiang()
{
}

void CTianJiangBaoXiang::addPlayer(Player* player)
{
    if (player)
        CActivity::addPlayer(player);
}

int32_t CTianJiangBaoXiang::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (player && pTargetMap)
        return CActivity::canEnter(player, pTargetMap);
    return 10002;
}

void CTianJiangBaoXiang::reset()
{
    CActivity::reset();
    GradeBaoXiangState = 0;
    m_PlayerSoreMap.clear();
}

void CTianJiangBaoXiang::sendPlayerScore(Player* player)
{
    if (!player)
        return;
    int32_t alreadGrageCount = 0;
    int8_t isGetSpecial = 0;
    int32_t buyCount = 0;
    CharId_t cid = Player::getCid(player);
    auto it = m_PlayerSoreMap.find(cid);
    if (it != m_PlayerSoreMap.end())
    {
        alreadGrageCount = it->second.OpenBaoXiangCount;
        isGetSpecial = it->second.IsGetSpecial;
        buyCount = it->second.BuyCount;
    }
    int8_t connId = Player::getConnId(player);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (packet)
    {
        packet->writeInt32(m_cfgActivity->id);
        packet->writeInt8(GradeBaoXiangState);
        packet->writeInt32(buyCount + 10 - alreadGrageCount);
        packet->writeInt8(isGetSpecial);
        packet->writeInt32(getLeftTime());
        packet->writeInt32(buyCount);
        packet->setSize(packet->getWOffset());
        int16_t gateIndex = Player::getGateIndex(player);
        int8_t connId2 = Player::getConnId(player);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
    }
}

void CTianJiangBaoXiang::AddPlant(PlantActivity* plant)
{
    if (!plant)
        return;
    if (Plant::GetPlantType(plant) == 1)
    {
        GradeBaoXiangState = 1;
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(21);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, 0, packet);
        }
    }
    for (auto p : m_players)
        sendPlayerScore(p);
}

int32_t CTianJiangBaoXiang::GiveDailyReward(Player* player)
{
    if (!player)
        return 2;
    ActivityRewardRecord rewardRecord;
    rewardRecord.OpenBaoXiangCount = 0;
    rewardRecord.IsGetSpecial = 0;
    rewardRecord.BuyCount = 0;
    CharId_t cid = Player::getCid(player);
    auto it = m_PlayerSoreMap.find(cid);
    if (it != m_PlayerSoreMap.end())
        rewardRecord = it->second;
    if (rewardRecord.IsGetSpecial)
        return 2;
    MemChrBagVector addItem;
    for (auto giftId : m_cfgActivity->gift_id)
    {
        MemChrBag stu;
        stu.itemId = giftId;
        stu.itemCount = 1;
        stu.bind = 1;
        addItem.push_back(stu);
    }
    CExtCharBag* bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(bag, &addItem, ITEM_CHANGE_REASON::ICR_TIAN_JIANG_BAO_XIANG_DAILY_REWARD))
        return 10002;
    rewardRecord.IsGetSpecial = 1;
    m_PlayerSoreMap[Player::getCid(player)] = rewardRecord;
    sendPlayerScore(player);
    return 0;
}

void CTianJiangBaoXiang::onPlantGather(PlantActivity* pPlant, Player* player)
{
    if (pPlant && player)
    {
        if (Plant::GetPlantType(pPlant) == 1)
        {
            GradeBaoXiangState = 2;
            for (auto p : m_players)
                sendPlayerScore(p);
        }
        else
        {
            ++m_PlayerSoreMap[Player::getCid(player)].OpenBaoXiangCount;
            sendPlayerScore(player);
        }
        LogActivity logActivity;
        logActivity.cid = Player::getCid(player);
        logActivity.actid = GetId();
        logActivity.acttype = GetType();
        logActivity.time = Unit::getNow(player);
        logActivity.param = Plant::getPlantId(pPlant);
        int8_t connId = Player::getConnId(player);
        DBService* db = Answer::Singleton<DBService>::instance();
        db->InsertActivityLog(connId, &logActivity);
    }
}

int32_t CTianJiangBaoXiang::onBeginGather(PlantActivity* plant, Player* player)
{
    if (!player || !plant)
        return 2;
    if (Plant::GetPlantType(plant) == 1)
        return 0;
    CharId_t cid = Player::getCid(player);
    auto it = m_PlayerSoreMap.find(cid);
    if (it == m_PlayerSoreMap.end())
        return 0;
    if (it->second.OpenBaoXiangCount < it->second.BuyCount + 10)
        return 0;
    return 2;
}

bool CTianJiangBaoXiang::AddOpenCount(Player* player, int32_t addCount)
{
    if (m_nState != ACTIVITY_STATE::AS_RUNNING || !player)
        return false;
    CharId_t cid = Player::getCid(player);
    auto it = m_PlayerSoreMap.find(cid);
    if (it == m_PlayerSoreMap.end())
        return false;
    if (addCount + it->second.BuyCount > 5)
        return false;
    it->second.BuyCount += addCount;
    return true;
}

void CTianJiangBaoXiang::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(142);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CTianJiangBaoXiang::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(487);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}
