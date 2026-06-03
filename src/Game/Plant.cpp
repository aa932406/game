#include "common.h"
#include "Game/Plant.h"
#include "Game/Map.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/CTimer.h"
#include "Other/CfgPlant.h"
#include "Other/CfgMapPlant.h"
#include "Other/EntityType.h"
#include "Other/StaticObj.h"
#include "Other/Entity.h"
#include "Other/Monster.h"
#include "Other/CExtCharBag.h"
#include "Other/CExtCharFamily.h"
#include "Other/CFestivalActivity.h"
#include "Other/CChrTask.h"
#include "Other/CfgPlantEvent.h"
#include "Other/MemChrBagVector.h"
#include "Other/ItemChangeReason.h"
#include "Utility/Position.h"

#include <cstring>
#include <algorithm>
#include <ctime>

Plant::Plant()
    : StaticObj(EntityType::ET_PLANT)
    , m_pSummonBoss(nullptr)
    , m_gather(0)
    , m_gatherTick(0)
    , m_isCorpse(0)
    , m_isHide(0)
    , m_corpseTick(0)
    , m_whoCanPlant(0)
    , m_nLifeTime(0)
    , m_Camp(0)
{
    memset(&m_cfgPlant, 0, sizeof(m_cfgPlant));
    memset(&m_cfgMapPlant, 0, sizeof(m_cfgMapPlant));
}

Plant::~Plant()
{
}

void Plant::init(Map *pMap, const CfgPlant *cfgPlant, const CfgMapPlant *cfgMapPlant, Monster *pBoss)
{
    m_cfgPlant = *cfgPlant;
    m_cfgMapPlant = *cfgMapPlant;
    m_pSummonBoss = pBoss;
    m_pMap = pMap;

    m_currentTile = Position(m_cfgMapPlant.x, m_cfgMapPlant.y);
    m_currentPos = Map::tileToPixel(m_cfgMapPlant.x, m_cfgMapPlant.y);

    m_gather = 0;
    m_gatherTick = 0;
    m_isCorpse = 0;
    m_isHide = 0;
    m_corpseTick = 0;
    m_whoCanPlant = 0;
    m_nLifeTime = 0;
    m_Camp = 0;
}

void Plant::reset()
{
    StaticObj::reset();
    m_gather = 0;
    m_gatherTick = 0;
    m_isCorpse = 0;
    m_isHide = 0;
    m_corpseTick = 0;
    m_whoCanPlant = 0;
    m_pSummonBoss = nullptr;
    m_Camp = 0;
}

void Plant::refresh()
{
    if (!m_pMap)
        return;

    tm localNow;
    Map::getLocalNow(&localNow, m_pMap);

    if (localNow.tm_hour < m_cfgPlant.start_hour || localNow.tm_hour > m_cfgPlant.end_hour)
    {
        m_isHide = 1;
        broadcastRemove();
    }
    else if (m_isCorpse && m_cfgPlant.revive_time > 0)
    {
        int64_t curTick = Map::getTick(m_pMap);

        if (!m_isHide && curTick - m_corpseTick >= m_cfgPlant.hide_time)
        {
            m_isHide = 1;
            broadcastRemove();

            if (m_cfgPlant.revive_skin > 0)
                Map::broadcastIntoMap(m_pMap, this);
        }

        if (!m_pSummonBoss && curTick - m_corpseTick >= m_cfgPlant.revive_time)
        {
            revive();
        }
    }
    else if (!m_pSummonBoss && m_isHide)
    {
        revive();
    }
}

int32_t Plant::GetReviveTime()
{
    if (m_cfgPlant.revive_skin > 0)
    {
        if (m_isCorpse)
        {
            int64_t revive_time = m_cfgPlant.revive_time;
            int64_t m_corpseTick = m_corpseTick;
            return static_cast<int32_t>((revive_time + m_corpseTick - Map::getTick(m_pMap)) / 1000);
        }
        return 0;
    }
    return 0;
}

int32_t Plant::onBeginGather(Player *player)
{
    if (!m_pMap || !player)
        return 10002;

    if (m_gather != 0)
        return 10002;

    // 检查任务
    int32_t TaskId = m_cfgPlant.task;
    if (TaskId > 0)
    {
        if (!ChrTask::IsTaskFinished(player, TaskId))
            return 10002;
    }

    // 检查家族
    if (m_cfgPlant.need_family > 0)
    {
        CExtCharFamily *CharFamily = Player::GetCharFamily(player);
        if (!CExtCharFamily::IsDeclareWarFamily(CharFamily, static_cast<int8_t>(m_cfgPlant.need_family)))
            return 10002;
    }

    // 检查活动
    if (m_cfgPlant.activity_id > 0)
    {
        // 检查对应活动是否激活
    }

    // 检查采集CD
    int32_t Record = 3020;
    if (Player::GetOperateLimit(player))
    {
        // 检查采集限制
    }

    m_gather = Entity::getId(player);
    m_gatherTick = Map::getTick(m_pMap);
    BroadcastChange();
    return 0;
}

int32_t Plant::onEndGather(Player *player)
{
    if (!m_pMap || !player)
        return 10002;

    if (static_cast<EntityId_t>(m_gather) != Entity::getId(player))
        return 10002;

    int64_t Tick = Map::getTick(m_pMap);
    int64_t gatherDuration = Tick - m_gatherTick;

    if (gatherDuration < m_cfgPlant.gather_time)
        return 10002;

    m_gather = 0;
    m_gatherTick = 0;

    // 随机事件
    int32_t EventMaxRate = m_cfgPlant.event_maxRate;
    if (EventMaxRate > 0)
    {
        int32_t randomVal = Answer::Random::rand(1, 100);
        if (randomVal <= EventMaxRate)
        {
            int32_t EventId = m_cfgPlant.event_id;
            if (EventId > 0)
            {
                return PlantEvent(player, EventId);
            }
        }
    }

    // 变为尸体
    m_isCorpse = 1;
    m_corpseTick = Tick;
    BroadcastChange();

    return 0;
}

int32_t Plant::PlantEvent(Player *pPlayer, int32_t EventId)
{
    if (!m_pMap || !pPlayer)
        return 10002;

    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgPlantEvent *pEvent = CfgData::GetPlantEvent(pCfgData, EventId);

    if (!pEvent)
        return 10002;

    switch (pEvent->nType)
    {
        case 1: // 直接获得物品
        {
            CExtCharBag *Bag = Player::GetBag(pPlayer);
            if (CExtCharBag::AddItem(Bag, &pEvent->vReward, ITEM_CHANGE_REASON::ICR_PLANT_GATHER))
            {
                SendGainItem(pPlayer, &pEvent->vReward);
            }
            break;
        }
        case 2: // 进入活动地图
        {
            // 处理进入活动地图逻辑
            break;
        }
        case 3: // 进入副本
        {
            // 处理进入副本逻辑
            break;
        }
        case 4: // 掉落物品到地面
        {
            if (m_pMap)
            {
                Map::addDropItemGroup(m_pMap, &pEvent->vReward, &m_currentTile, 0);
            }
            break;
        }
        case 5: // 召唤载具
        {
            // 处理召唤载具逻辑
            break;
        }
    }

    return 0;
}

void Plant::SendGainItem(Player *pPlayer, const MemChrBagVector *vItem)
{
    if (!pPlayer || !vItem)
        return;

    int8_t ConnId = Player::getConnId(pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, ConnId, Answer::PackType::PACK_DISPATCH, 0x63);
    if (!packet)
        return;

    // 写入物品信息
    for (size_t i = 0; i < vItem->size(); ++i)
    {
        Answer::NetPacket::writeInt32(packet, (*vItem)[i].nItemId);
        Answer::NetPacket::writeInt32(packet, (*vItem)[i].nCount);
    }

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int16_t GateIndex = Player::getGateIndex(pPlayer);
    int8_t connId = Player::getConnId(pPlayer);
    GameService *v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, connId, GateIndex, packet);
}

int32_t Plant::onBreakGather(Player *player)
{
    if (!m_pMap || !player)
        return 10002;

    BreakGather();
    return 0;
}

void Plant::BreakGather()
{
    m_gather = 0;
    m_gatherTick = 0;
    BroadcastChange();
}

bool Plant::IsShow()
{
    if (m_cfgPlant.revive_skin > 0)
        return true;

    if (m_cfgPlant.hide_time <= 0)
    {
        if (m_isCorpse)
            return false;
        if (m_isHide)
            return false;
    }
    else if (m_isHide)
    {
        return false;
    }

    return true;
}

bool Plant::appendInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return false;

    int8_t Type = Entity::getType(this);
    Answer::NetPacket::writeInt8(packet, Type);

    int64_t EntityId = Entity::getId(this);
    Answer::NetPacket::writeInt64(packet, EntityId);
    Answer::NetPacket::writeInt32(packet, m_cfgPlant.id);
    Answer::NetPacket::writeInt64(packet, m_gather);
    Answer::NetPacket::writeInt16(packet, m_cfgMapPlant.x);
    Answer::NetPacket::writeInt16(packet, m_cfgMapPlant.y);
    Answer::NetPacket::writeInt8(packet, m_Camp);

    int32_t ReviveTime = GetReviveTime();
    Answer::NetPacket::writeInt32(packet, ReviveTime);

    return true;
}

void Plant::BroadcastChange()
{
    if (!m_pMap)
        return;

    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x5F);
    if (!packet)
        return;

    int8_t Type = Entity::getType(this);
    Answer::NetPacket::writeInt8(packet, Type);

    int64_t EntityId = Entity::getId(this);
    Answer::NetPacket::writeInt64(packet, EntityId);
    Answer::NetPacket::writeInt32(packet, m_cfgPlant.id);
    Answer::NetPacket::writeInt64(packet, m_gather);
    Answer::NetPacket::writeInt16(packet, m_cfgMapPlant.x);
    Answer::NetPacket::writeInt16(packet, m_cfgMapPlant.y);
    Answer::NetPacket::writeInt8(packet, m_Camp);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Plant::BroadcastGathered()
{
    if (!m_pMap)
        return;

    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x60);
    if (!packet)
        return;

    int8_t Type = Entity::getType(this);
    Answer::NetPacket::writeInt8(packet, Type);

    int64_t EntityId = Entity::getId(this);
    Answer::NetPacket::writeInt64(packet, EntityId);
    Answer::NetPacket::writeInt32(packet, m_cfgPlant.id);
    Answer::NetPacket::writeInt64(packet, m_gather);
    Answer::NetPacket::writeInt16(packet, m_cfgMapPlant.x);
    Answer::NetPacket::writeInt16(packet, m_cfgMapPlant.y);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Plant::broadcastRemove()
{
    if (m_pMap)
        Map::broadcastLeaveMap(m_pMap, this);
}

int32_t Plant::getNow()
{
    if (m_pMap)
        return Map::getNow(m_pMap);

    CTimer *pTimer = Answer::Singleton<CTimer>::instance();
    return CTimer::GetNow(pTimer);
}

void Plant::getLocalNow(tm *retstr)
{
    if (m_pMap)
    {
        Map::getLocalNow(retstr, m_pMap);
    }
    else
    {
        CTimer *pTimer = Answer::Singleton<CTimer>::instance();
        CTimer::GetLocalNow(pTimer, retstr);
    }
}

bool Plant::isLifeEnd()
{
    return m_nLifeTime > 0 && getNow() >= m_nLifeTime;
}

void Plant::setLifeTime(int32_t nLifeTime)
{
    if (nLifeTime <= 0)
        m_nLifeTime = 0;
    else
        m_nLifeTime = nLifeTime + getNow();
}

void Plant::revive()
{
    m_gather = 0;
    m_gatherTick = 0;
    m_isCorpse = 0;
    m_isHide = 0;
    m_corpseTick = 0;

    if (m_cfgPlant.revive_skin > 0)
        broadcastRemove();

    Map::broadcastIntoMap(m_pMap, this);
}

int32_t Plant::GetSpecialItem(int32_t nIndex)
{
    if (nIndex < 0 || static_cast<int32_t>(m_cfgPlant.ItemVt.size()) <= nIndex)
        return 0;

    return m_cfgPlant.ItemVt[nIndex];
}
