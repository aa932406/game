#include "Other/CHorseRacing.h"
#include "Answer.h"
#include "GameService.h"
#include "MapManager.h"
#include "Player.h"
#include "Unit.h"
#include "DBService.h"
#include "CfgData.h"
#include "Monster.h"

CHorseRacing::CHorseRacing(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_nIndex = 0;
    m_BossDie = 0;
}

CHorseRacing::~CHorseRacing()
{
}

void CHorseRacing::reset()
{
    CActivity::reset();
    m_nIndex = 0;
    m_BossDie = 0;
    m_racing.clear();
    m_winner.clear();
    m_rankList.clear();
}

void CHorseRacing::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (pMap && Map::GetMapId(pMap) == 10003)
    {
        auto iter = m_racing.begin();
        while (iter != m_racing.end())
        {
            Player* player = *iter;
            if (!player)
            {
                iter = m_racing.erase(iter);
                continue;
            }
            if (StaticObj::getMapId(player) == Map::GetMapId(pMap))
            {
                Position pos(20, 57);
                Position currentTile = StaticObj::getCurrentTile(player);
                if (Position::tileDistance(&currentTile, &pos) <= 6)
                {
                    m_winner.push_back(player);
                    win(player);
                    iter = m_racing.erase(iter);
                    continue;
                }
            }
            ++iter;
        }
    }
}

void CHorseRacing::removePlayer(Player* player, bool islogout)
{
    CActivity::removePlayer(player, islogout);
    for (auto iter = m_racing.begin(); iter != m_racing.end(); ++iter)
    {
        if (*iter == player)
        {
            m_racing.erase(iter);
            break;
        }
    }
    for (auto iter = m_winner.begin(); iter != m_winner.end(); ++iter)
    {
        if (*iter == player)
        {
            m_winner.erase(iter);
            break;
        }
    }
    Unit::removeBuff(player, 136);
}

void CHorseRacing::addPlayer(Player* player)
{
    CActivity::addPlayer(player);
    m_racing.push_back(player);
    if (player && StaticObj::getMapId(player) != 90017)
        addActivityBuff(player, 136, 0);
}

int32_t CHorseRacing::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (!player || !pTargetMap)
        return 10002;
    if (Map::GetMapId(pTargetMap) == 90017)
    {
        if (!bArrived(Player::getCid(player)))
            return 10002;
    }
    else
    {
        if (bArrived(Player::getCid(player)))
        {
            Player::TiShiInfo(player, 4, 0);
            return 10002;
        }
    }
    return CActivity::canEnter(player, pTargetMap);
}

bool CHorseRacing::bArrived(CharId_t cid)
{
    for (auto& rank : m_rankList)
    {
        if (rank.nCharId == cid)
            return true;
    }
    return false;
}

void CHorseRacing::win(Player* player)
{
    if (player)
    {
        ++m_nIndex;
        addReward(player);
        sendActivityResult(player);
        broadcastWin(player);
        PlayerRank rank;
        rank.nCharId = Player::getCid(player);
        Player::getName(player, &rank.strName);
        m_rankList.push_back(rank);
        if (m_nIndex <= 3)
            setNeedBroadcastActivityScore();
        LogActivity logActivity;
        logActivity.param = 0;
        logActivity.cid = Player::getCid(player);
        logActivity.actid = 0;
        logActivity.acttype = GetType();
        logActivity.time = Unit::getNow(player);
        int8_t connId = Player::getConnId(player);
        DBService* db = Answer::Singleton<DBService>::instance();
        db->InsertActivityLog(connId, &logActivity);
    }
}

void CHorseRacing::addReward(Player* player)
{
    int32_t index = m_nIndex;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    const CfgHorseRacingRewardTable* rewardTable = CfgData::GetHorseRacingRewardTable(cfgData);
    const CfgHorseRacingReward* pReward = CfgHorseRacingRewardTable::GetReward(rewardTable, index);
    if (pReward && player)
    {
        if (pReward->nExp > 0)
            Player::addExp(player, pReward->nExp, 0, 1);
        if (pReward->nMoney > 0)
            Player::AddCurrency(player, CURRENCY_TYPE::CURRENCY_MONEY, pReward->nMoney, CURRENCY_CHANGE_REASON::MCR_HORSE_RACING_REWARD, 0);
        if (!pReward->Items.empty())
        {
            int32_t nMailIndex = 0;
            switch (m_nIndex)
            {
                case 1: nMailIndex = 6212; break;
                case 2: nMailIndex = 6213; break;
                case 3: nMailIndex = 6214; break;
                case 4: nMailIndex = 6240; break;
                case 5: nMailIndex = 6241; break;
                case 6: nMailIndex = 6242; break;
                case 7: nMailIndex = 6243; break;
                case 8: nMailIndex = 6244; break;
                case 9: nMailIndex = 6245; break;
                case 10: nMailIndex = 6246; break;
                default: nMailIndex = 6247; break;
            }
            CharId_t cid = Player::getCid(player);
            int8_t connId = Player::getConnId(player);
            DBService* db = Answer::Singleton<DBService>::instance();
            db->OnSendSysMail(connId, cid, nMailIndex, &pReward->Items, ITEM_CHANGE_REASON::ICR_HORCE_RACINT_REWARD, nullptr, 0);
        }
    }
}

void CHorseRacing::sendActivityResult(Player* player)
{
    if (player)
    {
        int8_t connId = Player::getConnId(player);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E27u);
        if (packet)
        {
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt32(m_nIndex);
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = Player::getGateIndex(player);
            int8_t connId2 = Player::getConnId(player);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}

void CHorseRacing::onTimeEnd()
{
    m_nState = ACTIVITY_STATE::AS_NOT_START;
    delayKickAll(0);
}

bool CHorseRacing::CanUsePet(MapId_t mid)
{
    return mid == 90017;
}

void CHorseRacing::onMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (pMonster && Monster::getMid(pMonster) == 476078)
    {
        m_BossDie = 1;
        setNeedBroadcastActivityScore();
    }
}

void CHorseRacing::packetActivityScore(int8_t connid)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connid, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (!packet)
        return;
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt32(getLeftTime());
    int16_t nCount = 0;
    uint32_t offset = packet->getWOffset();
    packet->writeInt16(0);
    for (auto& rank : m_rankList)
    {
        packet->writeUTF8(&rank.strName);
        if (++nCount > 2)
            break;
    }
    packet->writeInt8(m_BossDie ? 2 : 1);
    uint32_t oldOffset = packet->getWOffset();
    packet->setWOffset(offset);
    packet->writeInt16(nCount);
    packet->setWOffset(oldOffset);
    packet->setSize(packet->getWOffset());
}

void CHorseRacing::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(19);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CHorseRacing::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(20);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CHorseRacing::broadcastWin(Player* player)
{
    if (player)
    {
        int32_t nBroadCastId = 0;
        switch (m_nIndex)
        {
            case 1: nBroadCastId = 13; break;
            case 2: nBroadCastId = 14; break;
            case 3: nBroadCastId = 15; break;
        }
        if (nBroadCastId > 0)
        {
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
            if (packet)
            {
                packet->writeInt32(nBroadCastId);
                std::string name;
                Player::getName(player, &name);
                packet->writeUTF8(&name);
                packet->writeInt64(Player::getCid(player));
                packet->setSize(packet->getWOffset());
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                GameService::worldBroadcast(gs2, 0, packet);
            }
        }
    }
}
