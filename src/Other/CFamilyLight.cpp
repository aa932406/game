#include "Other/CFamilyLight.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"

CFamilyLight::CFamilyLight(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
}

CFamilyLight::~CFamilyLight()
{
}

void CFamilyLight::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (pMap)
    {
        int64_t curTick = Map::getTick(pMap);
        if (curTick - m_LastUpdateTick > 10000)
        {
            m_LastUpdateTick = curTick;
            std::set<int> posIdSet;
            for (auto& entry : m_mPlayerScore)
            {
                PlayerScore& score = entry.second;
                if (score.bInActivity)
                {
                    int32_t runnerId = Map::GetRunnerId(pMap);
                    GameService* gs = Answer::Singleton<GameService>::instance();
                    Player* player = GameService::getPlayer(gs, score.nCharId, runnerId, 1);
                    if (player)
                    {
                        if (StaticObj::getMapId(player) == Map::GetMapId(pMap))
                        {
                            int32_t y = StaticObj::getCurrentTile(player).y;
                            int32_t x = StaticObj::getCurrentTile(player).x;
                            CfgData* cfgData = Answer::Singleton<CfgData>::instance();
                            const CfgFamilyLightExpTable* expTable = CfgData::GetFamilyLightExpTable(cfgData);
                            const PassionBubbleCfg* pCfg = CfgFamilyLightExpTable::GetPassionBubbleCfg(expTable, x, y);
                            if (pCfg && Unit::isAlive(player))
                            {
                                if (pCfg->nId > 0)
                                {
                                    if (posIdSet.find(pCfg->nId) != posIdSet.end())
                                    {
                                        Player::TiShiInfo(player, 23, 0);
                                        continue;
                                    }
                                    posIdSet.insert(pCfg->nId);
                                }
                                Player::AddCurrency(player, CURRENCY_TYPE::CURRENCY_BIND_MONEY, pCfg->Money, CURRENCY_CHANGE_REASON::GCR_FAMILY_LIGHT, 0);
                                Player::AddCurrency(player, CURRENCY_TYPE::CURRENCY_BLOOD, pCfg->Score, CURRENCY_CHANGE_REASON::GCR_FAMILY_LIGHT, 0);
                                score.nScore += pCfg->Score;
                                score.nMoney += pCfg->Money;
                                AddScore(player, pCfg->Money, pCfg->Score);
                            }
                            sendPlayerScore(player);
                        }
                    }
                }
            }
        }
    }
}

void CFamilyLight::onPlayerKilled(Player* pDier, Player* pKiller)
{
    if (pDier && pKiller)
    {
        auto it = m_mPlayerScore.find(Player::getCid(pKiller));
        if (it != m_mPlayerScore.end())
        {
            ++it->second.nKillCount;
            uint32_t nKillCount = it->second.nKillCount;
            if (nKillCount <= 50 && ((1LL << nKillCount) & 0x4000040008020LL) != 0)
            {
                int8_t connId = Player::getConnId(pKiller);
                GameService* gs = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
                if (packet)
                {
                    packet->writeInt32(464);
                    std::string val;
                    Player::getName(pKiller, &val);
                    packet->writeUTF8(&val);
                    packet->writeInt64(Player::getCid(pKiller));
                    packet->writeInt32(it->second.nKillCount);
                    packet->setSize(packet->getWOffset());
                    int8_t connId2 = Player::getConnId(pKiller);
                    GameService* gs2 = Answer::Singleton<GameService>::instance();
                    GameService::worldBroadcast(gs2, connId2, packet);
                }
            }
        }
    }
}

void CFamilyLight::AddScore(Player* pPlayer, int32_t money, int32_t score)
{
    if (pPlayer)
    {
        int8_t connId = Player::getConnId(pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E2Fu);
        if (packet)
        {
            packet->writeInt32(money);
            packet->writeInt32(score);
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = Player::getGateIndex(pPlayer);
            int8_t connId2 = Player::getConnId(pPlayer);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}

bool CFamilyLight::OnSitRevive(Player* player)
{
    return player != nullptr;
}

void CFamilyLight::reset()
{
    CActivity::reset();
    m_mPlayerScore.clear();
    m_LastUpdateTick = 0;
}

void CFamilyLight::addPlayer(Player* player)
{
    if (player)
    {
        auto it = m_mPlayerScore.find(Player::getCid(player));
        if (it != m_mPlayerScore.end())
        {
            it->second.bInActivity = 1;
        }
        else
        {
            PlayerScore score;
            memset(&score, 0, sizeof(score));
            score.nCharId = Player::getCid(player);
            m_mPlayerScore[score.nCharId] = score;
        }
        if (Player::getFamilyId(player) <= 0)
            Player::setPkMode(player, 6, 0);
        else
            Player::setPkMode(player, 3, 0);
        CActivity::addPlayer(player);
    }
}

void CFamilyLight::removePlayer(Player* player, bool islogout)
{
    CActivity::removePlayer(player, islogout);
    if (player)
    {
        auto it = m_mPlayerScore.find(Player::getCid(player));
        if (it != m_mPlayerScore.end())
            it->second.bInActivity = 0;
    }
}

void CFamilyLight::sendPlayerScore(Player* player)
{
    if (player)
    {
        auto it = m_mPlayerScore.find(Player::getCid(player));
        if (it != m_mPlayerScore.end())
        {
            PlayerScore& score = it->second;
            int8_t connId = Player::getConnId(player);
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E26u);
            if (packet)
            {
                packet->writeInt32(m_cfgActivity->id);
                packet->writeInt32(score.nScore);
                packet->writeInt32(score.nMoney);
                packet->writeInt32(m_LastUpdateTick - Unit::getTick(player) + 10000);
                packet->writeInt32(getLeftTime());
                packet->setSize(packet->getWOffset());
                int16_t gateIndex = Player::getGateIndex(player);
                int8_t connId2 = Player::getConnId(player);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
            }
        }
    }
}

void CFamilyLight::broadcastActivityResult()
{
    for (auto& entry : m_mPlayerScore)
    {
        PlayerScore& score = entry.second;
        if (score.bInActivity)
        {
            GameService* gs = Answer::Singleton<GameService>::instance();
            Player* player = GameService::getPlayer(gs, score.nCharId, 0, 0);
            if (player)
            {
                int8_t connId = Player::getConnId(player);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(gs2, connId, Answer::PackType::PACK_DISPATCH, 0x2E27u);
                if (!packet)
                    return;
                packet->writeInt32(m_cfgActivity->id);
                packet->writeInt32(score.nScore);
                packet->writeInt32(score.nMoney);
                packet->setSize(packet->getWOffset());
                int16_t gateIndex = Player::getGateIndex(player);
                int8_t connId2 = Player::getConnId(player);
                GameService* gs3 = Answer::Singleton<GameService>::instance();
                GameService::sendPacketTo(gs3, connId2, gateIndex, packet);
            }
        }
    }
}

bool CFamilyLight::DrinkWine(Player* player, int32_t count)
{
    if (!player)
        return false;
    sendPlayerScore(player);
    return true;
}

void CFamilyLight::onTimeEnd()
{
    m_nState = ACTIVITY_STATE::AS_END;
    broadcastActivityResult();
    delayKickAll(0);
}

void CFamilyLight::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(128);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CFamilyLight::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(129);
        packet->writeInt32(GetId());
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}
