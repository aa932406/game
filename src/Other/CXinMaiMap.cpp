#include "common.h"
#include "Other/CXinMaiMap.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "MapManager.h"
#include "CTimer.h"
#include "Player.h"
#include "Unit.h"
#include "Monster.h"
#include "Plant.h"
#include "CExtOperateLimit.h"

CXinMaiMap::CXinMaiMap(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_LeftTime = 0;
    m_ReviveTimes = 0;
    m_pLastBoss = 0;
}

CXinMaiMap::~CXinMaiMap()
{
}

void CXinMaiMap::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
}

void CXinMaiMap::onPlayerKilled(Player* pDier, Player* pKiller)
{
}

bool CXinMaiMap::OnSitRevive(Player* player)
{
    if (!player)
        return false;
    int32_t time = m_ReviveTimes[Player::getCid(player)];
    if (time < 0)
        return false;
    if (!Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, 100 * (time + 1), CURRENCY_CHANGE_REASON::GCR_XIN_MAI_MAP_REVIVE, 0))
        return false;
    ++m_ReviveTimes[Player::getCid(player)];
    sendPlayerScore(player);
    return true;
}

int32_t CXinMaiMap::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (player && pTargetMap)
        return CActivity::canEnter(player, pTargetMap);
    return 10002;
}

void CXinMaiMap::reset()
{
    m_MonsterActivityMap.clear();
    m_pLastBoss = 0;
    m_LeftTime = 0;
    m_ReviveTimes.clear();
    CActivity::reset();
}

void CXinMaiMap::addPlayer(Player* player)
{
    if (player)
    {
        if (Player::getFamilyId(player) <= 0)
            Player::setPkMode(player, 6, 0);
        else
            Player::setPkMode(player, 3, 0);
        CActivity::addPlayer(player);
    }
}

void CXinMaiMap::removePlayer(Player* player, bool islogout)
{
    CActivity::removePlayer(player, islogout);
}

void CXinMaiMap::sendPlayerScore(Player* player)
{
    if (!player)
        return;
    int32_t count = 0;
    CharId_t cid = Player::getCid(player);
    auto it = m_ReviveTimes.find(cid);
    if (it != m_ReviveTimes.end())
        count = it->second;
    int8_t connId = Player::getConnId(player);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E24u);
    if (packet)
    {
        packet->writeInt32(m_cfgActivity->id);
        packet->writeInt32(count);
        packet->setSize(packet->getWOffset());
        int16_t gateIndex = Player::getGateIndex(player);
        int8_t connId2 = Player::getConnId(player);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
    }
}

void CXinMaiMap::packetActivityScore(int8_t connid)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (!packet)
        return;
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt32(m_MonsterActivityMap.size());
    for (auto& entry : m_MonsterActivityMap)
    {
        MonsterActivity* pMonster = entry.second;
        if (pMonster)
        {
            packet->writeInt64(Entity::getCid(pMonster));
            packet->writeInt32(Monster::getMid(pMonster));
            packet->writeInt32(!Unit::isAlive(pMonster) ? 1 : 0);
            packet->writeInt32(StaticObj::getMapId(pMonster));
            packet->writeInt32(StaticObj::GetPosX(pMonster));
            packet->writeInt32(StaticObj::GetPosY(pMonster));
        }
        else
        {
            packet->writeInt64(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
        }
    }
    if (m_pLastBoss)
    {
        packet->writeInt64(Entity::getCid(m_pLastBoss));
        packet->writeInt32(Monster::getMid(m_pLastBoss));
        packet->writeInt32(!Unit::isAlive(m_pLastBoss) ? 1 : 0);
        packet->writeInt32(StaticObj::getMapId(m_pLastBoss));
        packet->writeInt32(StaticObj::GetPosX(m_pLastBoss));
        packet->writeInt32(StaticObj::GetPosY(m_pLastBoss));
    }
    else
    {
        packet->writeInt64(0);
        packet->writeInt32(40030);
        packet->writeInt32(0);
        packet->writeInt32(60018);
        packet->writeInt32(108);
        packet->writeInt32(158);
    }
    packet->writeInt32(m_LeftTime);
    packet->writeInt32(getLeftTime());
    packet->setSize(packet->getWOffset());
}

void CXinMaiMap::onTimeEnd()
{
    m_nState = ACTIVITY_STATE::AS_END;
    broadcastActivityResult();
    delayKickAll(0);
}

void CXinMaiMap::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(491);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void CXinMaiMap::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(492);
        packet->writeInt32(GetId());
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void CXinMaiMap::onMonsterAdd(MonsterActivity* pMonster)
{
    if (pMonster && Monster::getActivityMonsterType(pMonster) == 1)
    {
        if (Monster::getBossSign(pMonster) == 14)
        {
            m_pLastBoss = pMonster;
            m_LeftTime = 0;
            setNeedBroadcastActivityScore();
        }
        else
        {
            m_MonsterActivityMap[Entity::getCid(pMonster)] = pMonster;
        }
    }
}

int32_t CXinMaiMap::onBeginGather(PlantActivity* plant, Player* player)
{
    if (!plant || !player)
        return 10002;
    if (Player::getRecord(player, 2101) <= 10)
        return 0;
    return 10002;
}

void CXinMaiMap::onPlantGather(PlantActivity* pPlant, Player* player)
{
    if (pPlant && player)
    {
        CExtOperateLimit* limit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(limit, 2101, 1);
    }
}

bool CXinMaiMap::alwaysShowIcon()
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    int32_t serverDiffDay = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);
    return GetDayBattle(serverDiffDay + 1) > 0;
}

void CXinMaiMap::onMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (pMonster && pKiller)
    {
        if (Monster::getActivityMonsterType(pMonster) && Monster::getBossSign(pMonster) != 14)
        {
            bool isAllDie = true;
            for (auto& entry : m_MonsterActivityMap)
            {
                MonsterActivity* pActivityMonster = entry.second;
                if (pActivityMonster && Unit::isAlive(pActivityMonster))
                {
                    isAllDie = false;
                    break;
                }
            }
            if (isAllDie)
                m_LeftTime = Unit::getNow(pKiller) + 180;
        }
        setNeedBroadcastActivityScore();
    }
}
