#include "common.h"
#include "Other/CPeerlessWar.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "MapManager.h"
#include "CExtCharTitle.h"
#include "Player.h"
#include "Unit.h"
#include "CActivityManager.h"
#include "DBService.h"
#include "CTimer.h"
#include "CExtCharBag.h"
#include "Map.h"
#include "ItemChangeReason.h"

CPeerlessWar::CPeerlessWar(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_nLastTick = 0;
    m_nStartTime = 0;
    m_nWarState = 0;
}

CPeerlessWar::~CPeerlessWar()
{
}

void CPeerlessWar::Init()
{
    CActivity::Init();
    Answer::DBPool* dbPool = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(dbPool);
    std::string sql = "SELECT a.`winner`,b.`name` FROM `mem_peerless_war` a LEFT JOIN `mem_character` b ON a.`winner`=b.`cid` ORDER BY a.`time` DESC LIMIT 1";
    Answer::MySqlQuery* q = Answer::MySqlDBGuard::query(&db, sql.c_str());
    Answer::MySqlQuery result(q);
    m_nWinner = Answer::MySqlQuery::getInt64Value(&result, "winner", 0);
    const char* name = Answer::MySqlQuery::getStringValue(&result, "name", "");
    m_strWinnerName = name;
    CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetPeerlessWarWinner(am, 0, m_nWinner);
}

void CPeerlessWar::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (pMap && m_nState == ACTIVITY_STATE::AS_RUNNING)
    {
        int64_t curTick = Map::getTick(pMap);
        if (curTick - m_nLastTick > 1000)
        {
            m_nLastTick = curTick;
            CTimer* timer = Answer::Singleton<CTimer>::instance();
            int32_t nNowTime = CTimer::GetNow(timer);
            if (m_nWarState == 1 && nNowTime - m_nStartTime > 300)
            {
                m_nWarState = 2;
                broadcastKnockout();
                checkWin();
                setNeedBroadcastActivityScore();
            }
        }
    }
}

void CPeerlessWar::reset()
{
    CActivity::reset();
    m_nLastTick = 0;
    m_nWarState = 0;
    m_mPlayerScore.clear();
}

void CPeerlessWar::onPlayerKilled(Player* pDier, Player* pAttacker)
{
    if (pDier && pAttacker)
    {
        CharId_t cid = Player::getCid(pDier);
        auto it = m_mPlayerScore.find(cid);
        if (it != m_mPlayerScore.end())
        {
            PlayerScore* score = &it->second;
            if (needBroadcastBreakCont(score->nBroadCont))
            {
                int32_t nBroadCont = score->nBroadCont;
                CharId_t dieCid = Player::getCid(pDier);
                std::string dieName;
                Player::getName(pDier, &dieName);
                CharId_t killerCid = Player::getCid(pAttacker);
                std::string killerName;
                Player::getName(pAttacker, &killerName);
                broadcastBreakConst(&killerName, killerCid, &dieName, dieCid, nBroadCont);
            }
            score->nBroadCont = 0;
            score->nContKill = 0;
            if (score->nReviveTimes > 2)
                Player::leaveActivity(pDier);
            addPlayerKillCount(pAttacker);
            checkWin();
        }
    }
}

void CPeerlessWar::onPlayerRevive(Player* player, bool bSafe)
{
    if (player && m_nWarState != 1)
    {
        CharId_t cid = Player::getCid(player);
        auto it = m_mPlayerScore.find(cid);
        if (it == m_mPlayerScore.end() || it->second.nReviveTimes > 2)
            Player::leaveActivity(player);
        else
        {
            ++it->second.nReviveTimes;
            sendPlayerScore(player);
        }
    }
}

void CPeerlessWar::addPlayer(Player* player)
{
    if (!player)
        return;
    CExtCharTitle* charTitle = Player::GetCharTitle(player);
    CExtCharTitle::ShowActivityTitle(charTitle);
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it != m_mPlayerScore.end())
        it->second.bInActivity = 1;
    else
    {
        PlayerScore score;
        memset(&score, 0, sizeof(score));
        score.nCharId = Player::getCid(player);
        Player::getName(player, &score.strName);
        score.bInActivity = 1;
        m_mPlayerScore[score.nCharId] = score;
    }
    CActivity::addPlayer(player);
    setNeedBroadcastActivityScore();
}

void CPeerlessWar::removePlayer(Player* player, bool islogout)
{
    if (player)
    {
        CExtCharTitle* charTitle = Player::GetCharTitle(player);
        CExtCharTitle::HideActivityTitle(charTitle, 1);
        CharId_t cid = Player::getCid(player);
        auto it = m_mPlayerScore.find(cid);
        if (it != m_mPlayerScore.end())
        {
            PlayerScore* score = &it->second;
            score->bInActivity = 0;
            if (m_nWarState == 2)
            {
                addJoinReward(score->nCharId);
                sendActivityResult(player, 0, score->nKillCount);
            }
        }
        CExtCharBag* bag = Player::GetBag(player);
        CExtCharBag::CleanItemId(bag, 574, ITEM_CHANGE_REASON::ICR_LEAVE_ACTIVE, 1);
        CActivity::removePlayer(player, islogout);
        setNeedBroadcastActivityScore();
        checkWin();
    }
}

void CPeerlessWar::sendPlayerScore(Player* player)
{
    if (!player)
        return;
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it == m_mPlayerScore.end())
        return;
    PlayerScore* score = &it->second;
    int8_t connId = Player::getConnId(player);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E24u);
    if (packet)
    {
        packet->writeInt32(m_cfgActivity->id);
        packet->writeInt32(3 - score->nReviveTimes);
        packet->writeInt32(score->nKillCount);
        packet->setSize(packet->getWOffset());
        int16_t gateIndex = Player::getGateIndex(player);
        int8_t connId2 = Player::getConnId(player);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
    }
}

void CPeerlessWar::packetActivityScore(int8_t connid)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connid, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (!packet)
        return;
    int32_t nLeftCount = m_players.size();
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt32(getLeftTime());
    packet->writeInt8(m_nWarState);
    packet->writeInt32(nLeftCount);
    packet->setSize(packet->getWOffset());
}

void CPeerlessWar::sendActivityResult(Player* player, int8_t nRes, int32_t KillCount)
{
    if (player)
    {
        int8_t connId = Player::getConnId(player);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E27u);
        if (packet)
        {
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt8(nRes);
            packet->writeInt32(KillCount);
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = Player::getGateIndex(player);
            int8_t connId2 = Player::getConnId(player);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}

Position CPeerlessWar::GetRandBornPos(Player* player)
{
    Position pos;
    if (player)
        return getBornRandPosA();
    Position::Position(&pos, -1, -1);
    return pos;
}

int32_t CPeerlessWar::getLeftReviveTimes(Player* player)
{
    if (!player)
        return 0;
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it == m_mPlayerScore.end())
        return 0;
    return 3 - it->second.nReviveTimes;
}

void CPeerlessWar::win(Player* pWinner)
{
    if (pWinner)
    {
        if (Player::getCid(pWinner) != m_nWinner)
            clearOldTitle();
        CExtCharTitle* charTitle = Player::GetCharTitle(pWinner);
        CExtCharTitle::CheckAddTitle(charTitle, 2, 1);
        m_nWinner = Player::getCid(pWinner);
        Player::getName(pWinner, &m_strWinnerName);
        CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
        CActivityManager::SetPeerlessWarWinner(am, 0, m_nWinner);
        broadcastWin(&m_strWinnerName, m_nWinner);
        m_nState = ACTIVITY_STATE::AS_END;
        m_nWarState = 3;
        saveWarResult();
        addWinnerReward();
        addRewards();
        CharId_t cid = Player::getCid(pWinner);
        auto it = m_mPlayerScore.find(cid);
        int32_t killerCount = 0;
        if (it != m_mPlayerScore.end())
            killerCount = it->second.nKillCount;
        sendActivityResult(pWinner, 1, killerCount);
        delayKickAll(30);
        OnActivityEnd();
        BroadcastActivityState();
    }
    else
        draw();
}

void CPeerlessWar::draw()
{
    clearOldTitle();
    m_nWinner = 0;
    m_strWinnerName.clear();
    m_nState = ACTIVITY_STATE::AS_END;
    m_nWarState = 3;
    addRewards();
    broadcastDraw();
    saveWarResult();
    delayKickAll(30);
    OnActivityEnd();
    BroadcastActivityState();
    CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetPeerlessWarWinner(am, 0, 0);
}

void CPeerlessWar::addWinnerReward()
{
    if (m_nWinner > 0)
    {
        MemChrBag reward;
        reward.itemId = m_cfgActivity->gift_id[0];
        reward.itemClass = 1;
        reward.itemCount = 1;
        reward.bind = 1;
        DBService* db = Answer::Singleton<DBService>::instance();
        db->OnSendSysMail(0, m_nWinner, 6258, &reward, ITEM_CHANGE_REASON::ICR_PEER_LESS_WAR_REWARD, nullptr, 0);
    }
}

void CPeerlessWar::addJoinReward(CharId_t nCharId)
{
    if (nCharId > 0)
    {
        MemChrBag reward;
        reward.itemId = m_cfgActivity->gift_id[1];
        reward.itemClass = 1;
        reward.itemCount = 1;
        reward.bind = 1;
        DBService* db = Answer::Singleton<DBService>::instance();
        db->OnSendSysMail(0, nCharId, 6259, &reward, ITEM_CHANGE_REASON::ICR_PEER_LESS_WAR_REWARD, nullptr, 0);
    }
}

void CPeerlessWar::addKillReward(CharId_t nCharId, int32_t nKillCount)
{
    if (nKillCount == 3)
    {
        MemChrBag reward;
        reward.itemId = m_cfgActivity->gift_id[2];
        reward.itemClass = 1;
        reward.itemCount = 1;
        reward.bind = 1;
        DBService* db = Answer::Singleton<DBService>::instance();
        db->OnSendSysMail(0, nCharId, 6260, &reward, ITEM_CHANGE_REASON::ICR_PEER_LESS_WAR_REWARD, nullptr, 0);
    }
    else if (nKillCount == 10)
    {
        MemChrBag reward;
        reward.itemId = m_cfgActivity->gift_id[3];
        reward.itemClass = 1;
        reward.itemCount = 1;
        reward.bind = 1;
        DBService* db = Answer::Singleton<DBService>::instance();
        db->OnSendSysMail(0, nCharId, 6261, &reward, ITEM_CHANGE_REASON::ICR_PEER_LESS_WAR_REWARD, nullptr, 0);
    }
}

void CPeerlessWar::addRewards()
{
    for (auto& entry : m_mPlayerScore)
    {
        PlayerScore* score = &entry.second;
        if (score->bInActivity && score->nCharId != m_nWinner)
        {
            addJoinReward(score->nCharId);
            GameService* gs = Answer::Singleton<GameService>::instance();
            Player* player = GameService::getPlayer(gs, score->nCharId, 0, 0);
            if (player)
                sendActivityResult(player, 0, score->nKillCount);
        }
    }
}

void CPeerlessWar::saveWarResult()
{
    int32_t startTime = m_nStartTime;
    std::string name = m_strWinnerName;
    CharId_t winner = m_nWinner;
    int32_t id = GetId();
    DBService* db = Answer::Singleton<DBService>::instance();
    db->SavePeerlessWarResult(id, winner, &name, startTime);
}

void CPeerlessWar::clearOldTitle()
{
    if (m_nWinner > 0)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        GameService::onRemoveTitle(gs, m_nWinner, 2);
    }
}

bool CPeerlessWar::needBroadcastContKill(int32_t nCountKill)
{
    return (uint32_t)nCountKill <= 0x14 && ((1LL << nCountKill) & 0x100420) != 0;
}

bool CPeerlessWar::needBroadcastBreakCont(int32_t nCountKill)
{
    return nCountKill == 10 || nCountKill == 20;
}

void CPeerlessWar::addPlayerKillCount(Player* player)
{
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it == m_mPlayerScore.end())
        return;
    PlayerScore* score = &it->second;
    ++score->nKillCount;
    ++score->nContKill;
    addKillReward(score->nCharId, score->nKillCount);
    if (needBroadcastContKill(score->nContKill))
    {
        score->nBroadCont = score->nContKill;
        int32_t nBroadCont = score->nBroadCont;
        CharId_t killerCid = Player::getCid(player);
        std::string name;
        Player::getName(player, &name);
        broadcastConstKill(&name, killerCid, nBroadCont);
    }
    sendPlayerScore(player);
}

void CPeerlessWar::onTimeEnd()
{
    draw();
    CActivity::onTimeEnd();
}

int32_t CPeerlessWar::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (m_nWarState == 1)
        return CActivity::canEnter(player, pTargetMap);
    return 10109;
}

void CPeerlessWar::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(175);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CPeerlessWar::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(176);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CPeerlessWar::broadcastKnockout()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(177);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CPeerlessWar::broadcastWin(const std::string* name, CharId_t cid)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(178);
        packet->writeUTF8(name);
        packet->writeInt64(cid);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CPeerlessWar::broadcastConstKill(const std::string* name, CharId_t cid, int32_t cont)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(180);
        packet->writeUTF8(name);
        packet->writeInt64(cid);
        packet->writeInt32(cont);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CPeerlessWar::broadcastBreakConst(const std::string* breaker_name, CharId_t breaker_cid, const std::string* name, CharId_t cid, int32_t cont)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(181);
        packet->writeUTF8(breaker_name);
        packet->writeInt64(breaker_cid);
        packet->writeUTF8(name);
        packet->writeInt64(cid);
        packet->writeInt32(cont);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CPeerlessWar::broadcastDraw()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(179);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

bool CPeerlessWar::checkWin()
{
    if (m_nState != ACTIVITY_STATE::AS_RUNNING || m_nWarState != 2)
        return false;
    Player* pWinner = 0;
    for (auto player : m_players)
    {
        if (player)
        {
            if (Unit::isAlive(player) || getLeftReviveTimes(player) > 0)
            {
                if (pWinner)
                    return false;
                pWinner = player;
            }
        }
    }
    win(pWinner);
    return true;
}

void CPeerlessWar::OnPeerlessWarResult(CharId_t winner, const std::string* name)
{
    m_nState = ACTIVITY_STATE::AS_END;
    m_nWinner = winner;
    m_strWinnerName = *name;
}

std::string CPeerlessWar::GetWinnerName()
{
    return m_strWinnerName;
}
