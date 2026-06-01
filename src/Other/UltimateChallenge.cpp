#include "Other/UltimateChallenge.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "MapManager.h"
#include "CTimer.h"
#include "Player.h"
#include "DBService.h"
#include "Monster.h"
#include "ItemChangeReason.h"

UltimateChallenge::UltimateChallenge(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_ActState = 0;
    m_lastUpdateTime = 0;
    m_nRand = 0;
    m_MonsterScore = 0;
}

UltimateChallenge::~UltimateChallenge()
{
}

void UltimateChallenge::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (pMap)
    {
        if (Map::getTick(pMap) - m_lastUpdateTime > 1000 && !m_ActState)
        {
            m_lastUpdateTime = Map::getTick(pMap);
            if (IsRuning() && getActivityTime() > 599)
            {
                m_ActState = 1;
                BroadcastActivityState();
            }
        }
    }
}

void UltimateChallenge::onMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (!pMonster || !pKiller)
        return;
    if (Monster::getActivityMonsterType(pMonster) != 1 || m_nRand <= 0)
        return;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    int32_t startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);
    if (startDays > 2)
        return;
    int32_t mapId = StaticObj::getMapId(pKiller);
    cfgData = Answer::Singleton<CfgData>::instance();
    UltimateChallengeCfg* pCfg = CfgData::GetUltimateChallengeCfgMap(cfgData, mapId);
    if (!pCfg || pCfg->Score <= 0)
        return;
    m_MonsterScore += pCfg->Score;
    if (m_MonsterScore < m_nRand)
        return;
    int32_t equip = 0;
    Answer::Random* rnd = Answer::Singleton<Answer::Random>::instance();
    int32_t nRand = Answer::Random::generate(rnd, 1, 10000);
    for (auto& drop : pCfg->DropRateList)
    {
        if (drop.nParam2 <= nRand)
        {
            equip = drop.nParam1;
            break;
        }
        nRand -= drop.nParam2;
    }
    if (equip >= 0)
    {
        DropItem item;
        item.itemClass = 2;
        item.itemId = equip;
        item.itemCount = 1;
        CDropItemGroup dropGroup;
        Map* pMap = StaticObj::getMap(pMonster);
        if (pMap)
        {
            Position pos = StaticObj::getCurrentTile(pMonster);
            int8_t bossSign = Monster::getBossSign(pMonster);
            int32_t monsterMid = Monster::getMid(pMonster);
            CDropItemGroup dropGroup2;
            dropGroup2.init(0, pMap, &pos, pKiller, 2, monsterMid, nullptr, nullptr, monsterMid, bossSign, 0);
            pMap->addDropItemGroup(&dropGroup2);
            m_nRand = 0;
        }
    }
}

void UltimateChallenge::BroadcastActivityState()
{
    int8_t nIconState = getIconState();
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CC3u);
    if (packet)
    {
        packet->writeInt32(m_cfgActivity->iconid);
        packet->writeInt8(nIconState);
        int32_t leftTime;
        if (nIconState == 2)
            leftTime = 600 - getActivityTime();
        else
            leftTime = getLeftTime();
        packet->writeInt32(leftTime);
        packet->writeInt8(0);
        packet->writeInt32(0);
        packet->writeInt8(nIconState == 2 ? 1 : 0);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

int8_t UltimateChallenge::GetState()
{
    if (m_nState == ACTIVITY_STATE::AS_RUNNING && getActivityTime() > 599)
        return 3;
    return m_nState;
}

void UltimateChallenge::reset()
{
    CActivity::reset();
    m_ActState = 0;
    m_lastUpdateTime = 0;
    m_nRand = 0;
    m_MonsterScore = 0;
}

int8_t UltimateChallenge::getIconState()
{
    if (m_nState == ACTIVITY_STATE::AS_RUNNING && getActivityTime() > 599)
        return 3;
    return m_nState;
}

int32_t UltimateChallenge::GetIconLeftTime()
{
    return 600 - getActivityTime();
}

int32_t UltimateChallenge::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (m_nState != ACTIVITY_STATE::AS_RUNNING)
        return 10002;
    if (Map::GetMapId(pTargetMap) == 50041 && m_ActState)
        return 10002;
    return CActivity::canEnter(player, pTargetMap);
}

void UltimateChallenge::removePlayer(Player* player, bool islogout)
{
    if (player)
    {
        int32_t mapId = StaticObj::getMapId(player);
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        UltimateChallengeCfg* pCfg = CfgData::GetUltimateChallengeCfgMap(cfgData, mapId);
        if (pCfg)
        {
            int32_t costItemId = pCfg->CostItems.m_nId;
            CExtCharBag* bag = Player::GetBag(player);
            CExtCharBag::CleanItemId(bag, costItemId, ITEM_CHANGE_REASON::ICR_LEAVE_ACTIVE, 1);
            CActivity::removePlayer(player, islogout);
        }
    }
}

void UltimateChallenge::addPlayer(Player* player)
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

void UltimateChallenge::onPlayerRevive(Player* player, bool bSafe)
{
    if (player && bSafe)
        Player::leaveActivity(player);
}

void UltimateChallenge::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(420);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void UltimateChallenge::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(419);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
        Answer::Random* rnd = Answer::Singleton<Answer::Random>::instance();
        m_nRand = Answer::Random::generate(rnd, 1, 10000);
    }
}

void UltimateChallenge::onPlayerKilled(Player* pDier, Player* pKiller)
{
}
