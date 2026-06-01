#include "Other/CFamilyWar.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "CActivityManager.h"
#include "Player.h"
#include "Unit.h"
#include "Monster.h"
#include "ChrTask.h"
#include "DBService.h"
#include "Map.h"

CFamilyWar::CFamilyWar(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_nActivePillarState = 0;
    m_nActiveState = 0;
    m_nBuffStartTick = 0;
    m_nWinFamily = 0;
    m_pTitle = 0;
    m_pBoss = 0;
    m_pBuffPlayer = 0;
}

CFamilyWar::~CFamilyWar()
{
}

void CFamilyWar::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (pMap)
    {
        int64_t curTick = Map::getTick(pMap);
        bool hasBuff = m_pBuffPlayer && StaticObj::getMap(m_pBuffPlayer) == pMap;
        if (hasBuff && curTick - m_nBuffStartTick > 299999)
        {
            std::string strFamilyName;
            Player::getFamilyName(m_pBuffPlayer, &strFamilyName);
            FamilyId_t familyId = Player::getFamilyId(m_pBuffPlayer);
            win(familyId, &strFamilyName);
            removeBuff(m_pBuffPlayer);
            m_pBuffPlayer = 0;
            setNeedBroadcastActivityScore();
        }
        for (auto& score : m_lstPlayerScore)
        {
            if (score.bInActivity && curTick - score.nStartTick > 9999)
            {
                int32_t runnerId = Map::GetRunnerId(pMap);
                Player* pPlayer = GameService::getPlayer(Answer::Singleton<GameService>::instance(), score.nCharId, runnerId, 1);
                if (pPlayer && StaticObj::getMap(pPlayer) == pMap)
                {
                    score.nStartTick = curTick;
                    int32_t level = Player::getLevel(pPlayer);
                    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
                    const CfgFamilyWarTable* warTable = CfgData::GetFamilyWarTable(cfgData);
                    int32_t nExp = CfgFamilyWarTable::GetJoinReward(warTable, level);
                    Player::addExp(pPlayer, nExp, 0, 1);
                    score.nExp += nExp;
                }
            }
        }
    }
}

void CFamilyWar::reset()
{
    CActivity::reset();
    m_nActiveState = 0;
    m_nActivePillarState = 0;
    m_pTitle = 0;
    m_pBoss = 0;
    m_pBuffPlayer = 0;
    m_nBuffStartTick = 0;
    m_nWinFamily = 0;
    m_strWingFamily.clear();
    m_lstFamilyScore.clear();
    m_lstPlayerScore.clear();
}

void CFamilyWar::onMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (pMonster && pKiller && IsRuning())
    {
        int32_t killPoints = Monster::GetKillPoints(pMonster);
        addPlayerScore(pKiller, killPoints, 0);
        if (m_pTitle == pMonster)
        {
            m_pBuffPlayer = pKiller;
            m_nBuffStartTick = Unit::getTick(pKiller);
            addBuff(m_pBuffPlayer);
            setNeedBroadcastActivityScore();
        }
    }
}

void CFamilyWar::onPlayerKilled(Player* pDier, Player* pAttacker)
{
    if (pDier && pAttacker)
    {
        addPlayerScore(pAttacker, 20, 1);
        ChrTask* task = Player::GetTask(pAttacker);
        ChrTask::updateTaskCount(task, 27, 0);
        if (m_pBuffPlayer == pDier)
        {
            m_pBuffPlayer = pAttacker;
            m_nBuffStartTick = Unit::getTick(pAttacker);
            addBuff(m_pBuffPlayer);
            setNeedBroadcastActivityScore();
        }
    }
}

void CFamilyWar::onMonsterAdd(MonsterActivity* pMonster)
{
    if (pMonster)
    {
        if (MonsterActivity::getActivityMonsterId(pMonster) == 20333)
            m_pTitle = pMonster;
        else if (MonsterActivity::getActivityMonsterId(pMonster) == 20334)
            m_pBoss = pMonster;
    }
}

void CFamilyWar::AdjustMonsterAttr(CfgMonster* cfgMonster)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    int32_t worldLevel = GameService::GetWorldLevel(gs);
    adjustMonsterAttr(cfgMonster, worldLevel, 1);
}

void CFamilyWar::addPlayer(Player* player)
{
    if (!player)
        return;
    if (Player::getFamilyId(player) <= 0)
        Player::setPkMode(player, 6, 0);
    else
        Player::setPkMode(player, 3, 0);
    int32_t nTaskId = 0;
    for (auto& score : m_lstPlayerScore)
    {
        if (score.nCharId == Player::getCid(player))
        {
            score.bInActivity = 1;
            nTaskId = score.nTaskId;
            break;
        }
    }
    auto endIter = m_lstPlayerScore.end();
    bool found = false;
    for (auto& score : m_lstPlayerScore)
    {
        if (score.nCharId == Player::getCid(player))
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        PlayerScore score;
        memset(&score, 0, 57);
        score.nCharId = Player::getCid(player);
        Player::getName(player, &score.strName);
        score.nFamilyId = Player::getFamilyId(player);
        int32_t level = Player::getLevel(player);
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        const CfgActivityTaskTable* taskTable = CfgData::GetActivityTaskTable(cfgData);
        score.nTaskId = CfgActivityTaskTable::RandomTask(taskTable, level);
        score.nStartTick = Unit::getTick(player);
        score.bInActivity = 1;
        nTaskId = score.nTaskId;
        m_lstPlayerScore.push_back(score);
    }
    if (nTaskId > 0)
    {
        ChrTask* task = Player::GetTask(player);
        ChrTask::receive(task, nTaskId);
    }
    CActivity::addPlayer(player);
}

void CFamilyWar::removePlayer(Player* player, bool islogout)
{
    CActivity::removePlayer(player, islogout);
    if (player)
    {
        if (Unit::HasBuffById(player, 100001))
        {
            removeBuff(player);
            if (m_pTitle)
                Monster::OnRevive(m_pTitle);
        }
        ChrTask* task = Player::GetTask(player);
        ChrTask::CleanTaskType(task, 41);
        for (auto& score : m_lstPlayerScore)
        {
            if (score.nCharId == Player::getCid(player))
            {
                score.bInActivity = 0;
                return;
            }
        }
    }
}

void CFamilyWar::addPlayerScore(Player* player, int32_t nScore, int32_t nKillCount)
{
    if (!player)
        return;
    CharId_t cid = Player::getCid(player);
    PlayerScore* pPlayerScore = getPlayerScore(cid);
    if (pPlayerScore)
    {
        if (nKillCount > 0)
        {
            if (pPlayerScore->nKillCount > 19)
                return;
            pPlayerScore->nKillCount += nKillCount;
        }
        pPlayerScore->nScore += nScore;
        std::string strFamilyName;
        Player::getFamilyName(player, &strFamilyName);
        FamilyId_t familyId = Player::getFamilyId(player);
        addFamilyScore(familyId, &strFamilyName, nScore);
        m_lstPlayerScore.sort(std::greater<PlayerScore>());
        sendPlayerScore(player);
        setNeedBroadcastActivityScore();
    }
}

void CFamilyWar::addFamilyScore(FamilyId_t nFamilyId, const std::string* strFamilyName, int32_t nScore)
{
    for (auto iter = m_lstFamilyScore.begin(); iter != m_lstFamilyScore.end(); ++iter)
    {
        if (iter->nFamilyId == nFamilyId)
        {
            iter->nScore += nScore;
            m_lstFamilyScore.sort(std::greater<FamilyScore>());
            setNeedBroadcastActivityScore();
            return;
        }
    }
    FamilyScore score;
    score.nFamilyId = nFamilyId;
    score.strFamilyName = *strFamilyName;
    score.nScore = nScore;
    m_lstFamilyScore.push_back(score);
    m_lstFamilyScore.sort(std::greater<FamilyScore>());
    setNeedBroadcastActivityScore();
}

void CFamilyWar::win(FamilyId_t nFamilyId, const std::string* strFamilyName)
{
    if (nFamilyId > 0)
    {
        m_nActiveState = 2;
        m_nWinFamily = nFamilyId;
        m_strWingFamily = *strFamilyName;
        sendWinnerReward(nFamilyId);
    }
    sendPlayerScoreRankReward();
    sendFamilyScoreRankReward();
    setNeedBroadcastActivityScore();
}

void CFamilyWar::sendPlayerScoreRankReward()
{
    int32_t nIndex = 1;
    for (auto iter = m_lstPlayerScore.begin(); iter != m_lstPlayerScore.end(); ++iter)
    {
        const PlayerScore* score = &(*iter);
        int32_t nGiftId = 0;
        int32_t nMailId = 0;
        if (nIndex == 1)
        {
            nGiftId = m_cfgActivity->gift_id[0];
            nMailId = 6336;
        }
        else if (nIndex > 1 && nIndex <= 3)
        {
            nGiftId = m_cfgActivity->gift_id[1];
            nMailId = 6337;
        }
        else
        {
            nGiftId = m_cfgActivity->gift_id[2];
            nMailId = 6338;
        }
        MemChrBag reward;
        reward.itemClass = 1;
        reward.itemId = nGiftId;
        reward.itemCount = 1;
        reward.bind = 1;
        CharId_t nCharId = score->nCharId;
        DBService* db = Answer::Singleton<DBService>::instance();
        db->OnSendSysMail(0, nCharId, nMailId, &reward, ITEM_CHANGE_REASON::ICR_FAMILY_WAR_REWARD, nullptr, 0);
        ++nIndex;
    }
}

void CFamilyWar::sendFamilyScoreRankReward()
{
    int32_t nIndex = 1;
    for (auto iter = m_lstFamilyScore.begin(); iter != m_lstFamilyScore.end(); ++iter)
    {
        const FamilyScore* score = &(*iter);
        int32_t nGiftId = 0;
        int32_t nMailId = 0;
        if (nIndex == 1)
        {
            nGiftId = m_cfgActivity->gift_id[3];
            nMailId = 6339;
        }
        else if (nIndex > 1 && nIndex <= 3)
        {
            nGiftId = m_cfgActivity->gift_id[4];
            nMailId = 6340;
        }
        else
        {
            nGiftId = m_cfgActivity->gift_id[5];
            nMailId = 6341;
        }
        MemChrBag reward;
        reward.itemClass = 1;
        reward.itemId = nGiftId;
        reward.itemCount = 1;
        reward.bind = 1;
        FamilyId_t nFamilyId = score->nFamilyId;
        DBService* db = Answer::Singleton<DBService>::instance();
        db->OnSendSysMail(0, 0, nMailId, &reward, ITEM_CHANGE_REASON::ICR_FAMILY_WAR_REWARD, nullptr, nFamilyId);
        ++nIndex;
    }
}

void CFamilyWar::sendActivePillarReward(FamilyId_t nFamilyId)
{
    MemChrBag reward;
    memset(&reward, 0, sizeof(reward));
    reward.itemId = m_cfgActivity->gift_id[6];
    reward.itemClass = 1;
    reward.itemCount = 1;
    reward.bind = 1;
    DBService* db = Answer::Singleton<DBService>::instance();
    db->OnSendSysMail(0, 0, 6342, &reward, ITEM_CHANGE_REASON::ICR_FAMILY_WAR_REWARD, nullptr, nFamilyId);
}

void CFamilyWar::sendWinnerReward(FamilyId_t nFamilyId)
{
    MemChrBag reward;
    memset(&reward, 0, sizeof(reward));
    reward.itemId = m_cfgActivity->gift_id[7];
    reward.itemClass = 1;
    reward.itemCount = 1;
    reward.bind = 1;
    DBService* db = Answer::Singleton<DBService>::instance();
    db->OnSendSysMail(0, 0, 6343, &reward, ITEM_CHANGE_REASON::ICR_FAMILY_WAR_REWARD, nullptr, nFamilyId);
}

void CFamilyWar::onTimeEnd()
{
    if (m_pBuffPlayer)
    {
        std::string strFamilyName;
        Player::getFamilyName(m_pBuffPlayer, &strFamilyName);
        FamilyId_t familyId = Player::getFamilyId(m_pBuffPlayer);
        win(familyId, &strFamilyName);
    }
    else
    {
        std::string empty;
        win(0, &empty);
    }
    CActivity::onTimeEnd();
    sendSocialUpdateActivityState(0);
}

void CFamilyWar::packetActivityScore(int8_t connid)
{
    std::string strFamilyName;
    int32_t nLeftWinTime = 0;
    if (m_pBuffPlayer)
    {
        Player::getFamilyName(m_pBuffPlayer, &strFamilyName);
        nLeftWinTime = (m_nBuffStartTick - Unit::getTick(m_pBuffPlayer) + 300000) / 1000;
        if (nLeftWinTime < 0)
            nLeftWinTime = 0;
    }
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connid, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (!packet)
        return;
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt8(m_nActiveState);
    packet->writeInt32(m_nActivePillarState);
    packet->writeInt32(getLeftTime());
    packet->writeUTF8(&strFamilyName);
    packet->writeInt32(nLeftWinTime);
    appendPlayerScoreRank(packet);
    appendFamilyScoreRank(packet);
    packet->setSize(packet->getWOffset());
}

void CFamilyWar::appendPlayerScoreRank(Answer::NetPacket* packet)
{
    if (!packet)
        return;
    int8_t nCount = 0;
    uint32_t offset = packet->getWOffset();
    packet->writeInt8(0);
    for (auto& score : m_lstPlayerScore)
    {
        if (score.nScore <= 0)
            break;
        packet->writeUTF8(&score.strName);
        packet->writeInt32(score.nScore);
        if (++nCount > 9)
            break;
    }
    uint32_t oldOffset = packet->getWOffset();
    packet->setWOffset(offset);
    packet->writeInt8(nCount);
    packet->setWOffset(oldOffset);
}

void CFamilyWar::appendFamilyScoreRank(Answer::NetPacket* packet)
{
    if (!packet)
        return;
    int8_t nCount = 0;
    uint32_t offset = packet->getWOffset();
    packet->writeInt8(0);
    for (auto& score : m_lstFamilyScore)
    {
        packet->writeUTF8(&score.strFamilyName);
        packet->writeInt32(score.nScore);
        if (++nCount > 2)
            break;
    }
    uint32_t oldOffset = packet->getWOffset();
    packet->setWOffset(offset);
    packet->writeInt8(nCount);
    packet->setWOffset(oldOffset);
}

void CFamilyWar::sendPlayerScore(Player* player)
{
    if (!player)
        return;
    int8_t nIndex = 0;
    int32_t nScore = 0;
    int8_t nTaskCount = 0;
    for (auto& score : m_lstPlayerScore)
    {
        ++nIndex;
        if (score.nCharId == Player::getCid(player))
        {
            nScore = score.nScore;
            nTaskCount = score.nTaskCount;
            break;
        }
    }
    int8_t nFamilyIndex = 0;
    int32_t nFamilyScore = 0;
    for (auto& score : m_lstFamilyScore)
    {
        ++nFamilyIndex;
        if (score.nFamilyId == Player::getFamilyId(player))
        {
            nFamilyScore = score.nScore - score.nUsedScore;
            break;
        }
    }
    int8_t connId = Player::getConnId(player);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E24u);
    if (packet)
    {
        packet->writeInt32(m_cfgActivity->id);
        packet->writeInt8(nIndex);
        packet->writeInt32(nScore);
        packet->writeInt8(nFamilyIndex);
        packet->writeInt32(nFamilyScore);
        packet->writeInt8(nTaskCount);
        packet->setSize(packet->getWOffset());
        int16_t gateIndex = Player::getGateIndex(player);
        int8_t connId2 = Player::getConnId(player);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
    }
}

void CFamilyWar::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(2);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CFamilyWar::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(3);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CFamilyWar::broadcastPillerKilled(FamilyId_t nFamilyId)
{
    if (nFamilyId <= 0)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(7);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, 0, packet);
        }
    }
    else
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(6);
            FamilyManager* fm = Answer::Singleton<FamilyManager>::instance();
            FamilyInfo info;
            FamilyManager::GetFamilyInfo(&info, fm, nFamilyId);
            packet->writeUTF8(&info.strName);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, 0, packet);
        }
    }
}

void CFamilyWar::broadcastStoneKilled()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(8);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CFamilyWar::addBuff(Player* player)
{
    if (player)
        addActivityBuff(player, 100001, 1);
}

void CFamilyWar::removeBuff(Player* player)
{
    if (player)
        Unit::removeBuff(player, 100001);
}

bool CFamilyWar::OnActivePillar(Player* player, int32_t nIndex)
{
    if (!player)
        return false;
    FamilyId_t familyId = Player::getFamilyId(player);
    FamilyScore* pFamilyScore = getFamilyScore(familyId);
    if (!pFamilyScore)
        return false;
    if (pFamilyScore->nScore - pFamilyScore->nUsedScore <= 999)
        return false;
    int32_t nResult = m_nActivePillarState | (1 << nIndex);
    if (m_nActivePillarState == nResult)
        return false;
    pFamilyScore->nUsedScore += 1000;
    ++pFamilyScore->nActivePillar;
    FamilyId_t fid = Player::getFamilyId(player);
    sendActivePillarReward(fid);
    m_nActivePillarState = nResult;
    setNeedBroadcastActivityScore();
    if (m_nActivePillarState == 15)
        m_nActiveState = 1;
    return true;
}

void CFamilyWar::OnTaskSubmited(Player* player, int32_t nScore)
{
    if (!player)
        return;
    CharId_t cid = Player::getCid(player);
    PlayerScore* pPlayerScore = getPlayerScore(cid);
    if (pPlayerScore)
    {
        pPlayerScore->nTaskId = 0;
        ++pPlayerScore->nTaskCount;
        pPlayerScore->nScore += nScore;
        if (pPlayerScore->nTaskCount == 19)
            pPlayerScore->nTaskId = 401009;
        else if (pPlayerScore->nTaskCount <= 19)
        {
            int32_t level = Player::getLevel(player);
            CfgData* cfgData = Answer::Singleton<CfgData>::instance();
            const CfgActivityTaskTable* taskTable = CfgData::GetActivityTaskTable(cfgData);
            pPlayerScore->nTaskId = CfgActivityTaskTable::RandomTask(taskTable, level);
        }
        if (pPlayerScore->nTaskId > 0)
        {
            int32_t nTaskId = pPlayerScore->nTaskId;
            ChrTask* task = Player::GetTask(player);
            ChrTask::receive(task, nTaskId);
        }
        m_lstPlayerScore.sort(std::greater<PlayerScore>());
        std::string strFamilyName;
        Player::getFamilyName(player, &strFamilyName);
        FamilyId_t familyId = Player::getFamilyId(player);
        addFamilyScore(familyId, &strFamilyName, nScore);
        sendPlayerScore(player);
    }
}

int32_t CFamilyWar::GetActivePillar(FamilyId_t nFamilyId)
{
    FamilyScore* pFamilyScore = getFamilyScore(nFamilyId);
    if (pFamilyScore)
        return pFamilyScore->nActivePillar;
    return 0;
}

int32_t CFamilyWar::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (!player || !pTargetMap)
        return 10002;
    if (Player::getFamilyId(player) > 0)
        return CActivity::canEnter(player, pTargetMap);
    return 10102;
}

CFamilyWar::PlayerScore* CFamilyWar::getPlayerScore(CharId_t nCharId)
{
    for (auto& score : m_lstPlayerScore)
    {
        if (score.nCharId == nCharId)
            return &score;
    }
    return 0;
}

CFamilyWar::FamilyScore* CFamilyWar::getFamilyScore(FamilyId_t nFamilyId)
{
    for (auto& score : m_lstFamilyScore)
    {
        if (score.nFamilyId == nFamilyId)
            return &score;
    }
    return 0;
}
