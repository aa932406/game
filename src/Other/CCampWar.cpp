#include "Other/CCampWar.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "MapManager.h"
#include "CExtCharTitle.h"

CCampWar::CCampWar(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_nLastTime = 0;
    m_BuffIdMap.clear();
    m_mPlayerScore.clear();
    m_sScoreRank.clear();
    m_Camp_1_battle = 0;
    m_Camp_2_battle = 0;
}

CCampWar::~CCampWar()
{
}

void CCampWar::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (pMap && IsRuning())
    {
        int32_t curTick = Map::getNow(pMap);
        if (curTick - m_nLastTime > 299)
        {
            m_nLastTime = curTick;
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
            if (packet)
            {
                packet->writeInt32(421);
                packet->setSize(packet->getWOffset());
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                GameService::worldBroadcast(gs2, 0, packet);
                setNeedBroadcastActivityScore();
                for (auto pScore : m_sScoreRank)
                {
                    if (pScore->nRankIndex && pScore->bInActivity)
                    {
                        if ((pScore->nRankIndex & 1) != 0)
                        {
                            pScore->nCamp = 2;
                            int32_t runnerId = Map::GetRunnerId(pMap);
                            GameService* gs3 = Answer::Singleton<GameService>::instance();
                            Player* player = GameService::getPlayer(gs3, pScore->nCharId, runnerId, 1);
                            if (player)
                                Player::SetCamp(player, 2);
                        }
                        else
                        {
                            pScore->nCamp = 1;
                            int32_t runnerId = Map::GetRunnerId(pMap);
                            GameService* gs3 = Answer::Singleton<GameService>::instance();
                            Player* player = GameService::getPlayer(gs3, pScore->nCharId, runnerId, 1);
                            if (player)
                                Player::SetCamp(player, 1);
                        }
                    }
                }
            }
        }
    }
}

void CCampWar::reset()
{
    CActivity::reset();
    m_nLastTime = 0;
    m_BuffIdMap.clear();
    m_mPlayerScore.clear();
    m_sScoreRank.clear();
    m_Camp_1_battle = 0;
    m_Camp_2_battle = 0;
}

void CCampWar::onPlayerKilled(Player* pDier, Player* pAttacker)
{
    if (pDier && pAttacker)
    {
        auto iter = m_mPlayerScore.find(Player::getCid(pDier));
        if (iter != m_mPlayerScore.end())
        {
            PlayerScore& score = iter->second;
            m_sScoreRank.erase(&score);
            if (score.nContLevel > 0)
            {
                CExtCharTitle* charTitle = Player::GetCharTitle(pDier);
                CExtCharTitle::SetActivityTitle(charTitle, 0, 0);
                int32_t nContLevel = score.nContLevel;
                CharId_t cid = Player::getCid(pAttacker);
                std::string breaker_name;
                Player::getName(pDier, &breaker_name);
                CharId_t v6 = Player::getCid(pDier);
                std::string name;
                Player::getName(pDier, &name);
                broadcastBreakContKill(&name, v6, &breaker_name, cid, nContLevel);
            }
            int32_t nPoint = 5;
            int32_t addOtherPoint = 20 * score.nPoints / 100;
            nPoint = addOtherPoint + 5;
            score.nPoints -= addOtherPoint;
            m_sScoreRank.insert(&score);
            addPlayerScore(pAttacker, nPoint, 1);
            notifyAddScore(pAttacker, nPoint, 1);
            score.nContKill = 0;
            score.nContLevel = 0;
            auto attIt = m_mPlayerScore.find(Player::getCid(pAttacker));
            if (attIt != m_mPlayerScore.end())
            {
                int32_t nKillCount = attIt->second.nKillCount;
                CharId_t first = attIt->first;
                addKillReward(first, nKillCount);
            }
        }
    }
}

void CCampWar::onMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (pMonster && pKiller)
    {
        addPlayerScore(pKiller, 5, 0);
        notifyAddScore(pKiller, 5, 2);
    }
}

void CCampWar::addKillReward(CharId_t nCharId, int32_t nKillCount)
{
    int32_t mailId = 0;
    MemChrBag reward;
    memset(&reward, 0, sizeof(reward));
    reward.itemId = 1;
    reward.itemCount = 1;

    switch (nKillCount)
    {
        case 1:
            reward.itemId = m_cfgActivity->gift_id[0];
            mailId = 6256;
            goto LABEL_SEND;
        case 5:
            reward.itemId = m_cfgActivity->gift_id[1];
            mailId = 6256;
            goto LABEL_SEND;
        case 10:
            reward.itemId = m_cfgActivity->gift_id[2];
            mailId = 6256;
            goto LABEL_SEND;
        case 20:
            reward.itemId = m_cfgActivity->gift_id[3];
            mailId = 6256;
            goto LABEL_SEND;
        case 50:
            reward.itemId = m_cfgActivity->gift_id[4];
            mailId = 6256;
LABEL_SEND:
            {
                DBService* db = Answer::Singleton<DBService>::instance();
                std::string param;
                DBService::OnSendSysMail(db, 0, nCharId, mailId, &reward, ITEM_CHANGE_REASON::ICR_PEER_LESS_WAR_REWARD, &param, 0);
            }
            break;
        default:
            return;
    }
}

void CCampWar::onPlayerRevive(Player* player, bool bSafe)
{
    if (player)
    {
        for (auto& entry : m_BuffIdMap)
        {
            if (entry.second == Player::GetCamp(player))
                addActivityBuff(player, entry.first, 1);
        }
    }
}

void CCampWar::onPlantGather(PlantActivity* pPlant, Player* player)
{
    if (pPlant && player && Player::GetCamp(player) > 0)
    {
        int32_t points = Plant::GetPoints(pPlant);
        addPlayerScore(player, points, 0);
        int32_t v4 = Plant::GetPoints(pPlant);
        notifyAddScore(player, v4, 0);
        int8_t camp = Player::GetCamp(player);
        Plant::SetCamp(pPlant, camp);
        int32_t buffId = Plant::GetBuffId(pPlant);
        m_BuffIdMap[buffId] = Player::GetCamp(player);
        int8_t v6 = Player::GetCamp(player);
        int32_t buffId2 = Plant::GetBuffId(pPlant);
        RefreshBuff(buffId2, v6);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(422);
            packet->writeInt64(Player::getCid(player));
            std::string val;
            Player::getName(player, &val);
            packet->writeUTF8(&val);
            packet->writeInt8(Player::GetCamp(player));
            packet->writeInt32(Plant::getPlantId(pPlant));
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, 0, packet);
        }
    }
}

void CCampWar::RefreshBuff(int32_t buffId, int8_t campId)
{
    for (auto pPlayer : m_players)
    {
        if (Player::GetCamp(pPlayer) == campId)
            addActivityBuff(pPlayer, buffId, 1);
        else
            Unit::removeBuff(pPlayer, buffId);
    }
}

void CCampWar::broadcastActivityResult()
{
    for (auto& entry : m_mPlayerScore)
    {
        PlayerScore& score = entry.second;
        if (score.bInActivity)
        {
            GameService* gs = Answer::Singleton<GameService>::instance();
            Player* pPlayer = GameService::getPlayer(gs, score.nCharId, 0, 0);
            if (pPlayer)
            {
                int8_t connId = Player::getConnId(pPlayer);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(gs2, connId, Answer::PackType::PACK_DISPATCH, 0x2E27u);
                if (!packet)
                    return;
                packet->writeInt32(m_cfgActivity->id);
                packet->writeInt32(score.nRankIndex);
                packet->writeInt32(score.nKillCount);
                packet->setSize(packet->getWOffset());
                int16_t gateIndex = Player::getGateIndex(pPlayer);
                int8_t connId2 = Player::getConnId(pPlayer);
                GameService* gs3 = Answer::Singleton<GameService>::instance();
                GameService::sendPacketTo(gs3, connId2, gateIndex, packet);
            }
        }
    }
}

int32_t CCampWar::onBeginGather(PlantActivity* pPlant, Player* player)
{
    if (!pPlant || !player)
        return 10002;
    if (Plant::GetCamp(pPlant) && Player::GetCamp(player) == Plant::GetCamp(pPlant))
        return 10002;
    return 0;
}

void CCampWar::addPlayer(Player* player)
{
    if (player)
    {
        CExtCharTitle* charTitle = Player::GetCharTitle(player);
        CExtCharTitle::ShowActivityTitle(charTitle);
        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            PlayerScore& score = iter->second;
            Player::SetCamp(player, score.nCamp);
            if (score.nContLevel > 0)
            {
                int32_t contTitle = getContTitle(score.nContLevel);
                CExtCharTitle* ct = Player::GetCharTitle(player);
                CExtCharTitle::SetActivityTitle(ct, 0, contTitle);
            }
            score.bInActivity = 1;
        }
        else
        {
            PlayerScore score;
            memset(&score, 0, sizeof(score));
            score.nCharId = Player::getCid(player);
            Player::getName(player, &score.strName);
            score.nAddTime = Unit::getNow(player);
            score.nCamp = selectCamp(player);
            score.nRankIndex = 0;
            score.bInActivity = 1;
            Player::SetCamp(player, score.nCamp);
            m_mPlayerScore[score.nCharId] = score;
            if (score.nCamp == 1)
                m_Camp_1_battle += player->getBattlePower();
            else
                m_Camp_2_battle += player->getBattlePower();
        }
        for (auto& entry : m_BuffIdMap)
        {
            if (entry.second == Player::GetCamp(player))
                addActivityBuff(player, entry.first, 1);
        }
        CActivity::addPlayer(player);
    }
}

void CCampWar::removePlayer(Player* player, bool islogout)
{
    CActivity::removePlayer(player, islogout);
    if (player)
    {
        Player::SetCamp(player, 0);
        CExtCharTitle* charTitle = Player::GetCharTitle(player);
        CExtCharTitle::HideActivityTitle(charTitle, 1);
        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            m_sScoreRank.erase(&iter->second);
            iter->second.bInActivity = 0;
            iter->second.nPoints -= (int32_t)((double)iter->second.nPoints * 0.2);
            m_sScoreRank.insert(&iter->second);
            refreshRankIndex(iter->second.nCharId);
            setNeedBroadcastActivityScore();
        }
    }
}

void CCampWar::addPlayerScore(Player* player, int32_t nPoint, int32_t nKillCount)
{
    if (player)
    {
        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            m_sScoreRank.erase(&iter->second);
            PlayerScore& score = iter->second;
            score.nPoints += nPoint;
            if (nKillCount > 0)
            {
                score.nKillCount += nKillCount;
                score.nContKill += nKillCount;
                CfgData* cfgData = Answer::Singleton<CfgData>::instance();
                const CfgCampWarTable* campWarTable = CfgData::GetCampWarTable(cfgData);
                int32_t nContLevel = CfgCampWarTable::GetContKillLevel(campWarTable, score.nContKill);
                if (score.nContLevel != nContLevel)
                {
                    score.nContLevel = nContLevel;
                    std::string name;
                    Player::getName(player, &name);
                    broadcastContKill(&name, Player::getCid(player), nContLevel);
                    int32_t contTitle = getContTitle(score.nContLevel);
                    CExtCharTitle* charTitle = Player::GetCharTitle(player);
                    CExtCharTitle::SetActivityTitle(charTitle, 0, contTitle);
                }
            }
            m_sScoreRank.insert(&iter->second);
            refreshRankIndex(Player::getCid(player));
            sendPlayerScore(player);
            setNeedBroadcastActivityScore();
            if (score.nPoints > 1999)
                onWarEnd();
        }
    }
}

void CCampWar::refreshRankIndex(CharId_t nCharId)
{
    int32_t nIndex = 1;
    for (auto pScore : m_sScoreRank)
    {
        if (pScore)
        {
            if (pScore->nRankIndex != nIndex)
            {
                pScore->nRankIndex = nIndex;
                if (pScore->bInActivity && pScore->nCharId != nCharId)
                {
                    GameService* gs = Answer::Singleton<GameService>::instance();
                    Player* player = GameService::getPlayer(gs, pScore->nCharId, 0, 0);
                    if (player)
                        sendPlayerScore(player);
                }
            }
            ++nIndex;
        }
    }
}

void CCampWar::addRewards()
{
    OnActivityEnd();
    for (auto& entry : m_mPlayerScore)
    {
        const PlayerScore& score = entry.second;
        int32_t nRankIndex = score.nRankIndex;
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        const CfgCampWarTable* campWarTable = CfgData::GetCampWarTable(cfgData);
        const CfgCampWarReward* pReward = CfgCampWarTable::GetReward(campWarTable, nRankIndex);
        if (pReward)
        {
            DBService* db = Answer::Singleton<DBService>::instance();
            std::string param;
            DBService::OnSendSysMail(db, 0, score.nCharId, pReward->nWinMailId, &pReward->WinItems, ITEM_CHANGE_REASON::ICR_CAMP_WAR_REWARD, &param, 0);
        }
    }
}

void CCampWar::onTimeEnd()
{
    addRewards();
    m_nState = ACTIVITY_STATE::AS_END;
    delayKickAll(0);
    OnActivityFinish();
}

void CCampWar::onWarEnd()
{
    addRewards();
    m_nState = ACTIVITY_STATE::AS_END;
    delayKickAll(0);
    OnActivityFinish();
}

int32_t CCampWar::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (player && pTargetMap)
        return CActivity::canEnter(player, pTargetMap);
    return 10002;
}

void CCampWar::packetActivityScore(int8_t connid)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connid, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (!packet)
        return;
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt32(getLeftTime());
    packet->writeInt32(m_nLastTime);
    int8_t nCount = 0;
    uint32_t offset = packet->getWOffset();
    packet->writeInt8(0);
    for (auto pScore : m_sScoreRank)
    {
        if (pScore)
        {
            packet->writeInt16(pScore->nRankIndex);
            packet->writeUTF8(&pScore->strName);
            packet->writeInt8(pScore->nCamp);
            packet->writeInt32(pScore->nPoints);
            packet->writeInt32(pScore->nKillCount);
            if (++nCount > 2)
                break;
        }
    }
    uint32_t oldOffset = packet->getWOffset();
    packet->setWOffset(offset);
    packet->writeInt8(nCount);
    packet->setWOffset(oldOffset);
    packet->setSize(packet->getWOffset());
}

void CCampWar::sendPlayerScore(Player* player)
{
    if (player)
    {
        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            const PlayerScore& score = iter->second;
            int8_t connId = Player::getConnId(player);
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E24u);
            if (packet)
            {
                packet->writeInt32(m_cfgActivity->id);
                packet->writeInt8(score.nCamp);
                packet->writeInt32(score.nPoints);
                packet->writeInt32(score.nKillCount);
                packet->writeInt16(score.nRankIndex);
                packet->setSize(packet->getWOffset());
                int16_t gateIndex = Player::getGateIndex(player);
                int8_t connId2 = Player::getConnId(player);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
            }
        }
    }
}

void CCampWar::SendPlayerRankInfo(Player* player)
{
    if (player)
    {
        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            const PlayerScore& score = iter->second;
            int8_t connId = Player::getConnId(player);
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E2Au);
            if (packet)
            {
                packet->writeInt32(m_cfgActivity->id);
                int16_t nCount = 0;
                uint32_t offset = packet->getWOffset();
                packet->writeInt16(nCount);
                for (auto pScore : m_sScoreRank)
                {
                    if (pScore && pScore->nPoints > 0)
                    {
                        packet->writeUTF8(&pScore->strName);
                        packet->writeInt8(pScore->nCamp);
                        packet->writeInt32(pScore->nPoints);
                        packet->writeInt32(pScore->nKillCount);
                        ++nCount;
                    }
                }
                uint32_t oldOffset = packet->getWOffset();
                packet->setWOffset(offset);
                packet->writeInt16(nCount);
                packet->setWOffset(oldOffset);
                packet->setSize(packet->getWOffset());
                int16_t gateIndex = Player::getGateIndex(player);
                int8_t connId2 = Player::getConnId(player);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
            }
        }
    }
}

void CCampWar::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(169);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CCampWar::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(170);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CCampWar::broadcastContKill(const std::string* name, CharId_t cid, int32_t nContLevel)
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    const CfgCampWarTable* campWarTable = CfgData::GetCampWarTable(cfgData);
    const CfgCampWarContKill* pContKill = CfgCampWarTable::GetContKill(campWarTable, nContLevel);
    if (pContKill)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(pContKill->nBroadcast);
            packet->writeUTF8(name);
            packet->writeInt64(cid);
            packet->writeInt32(pContKill->nKillCount);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, 0, packet);
        }
    }
}

void CCampWar::broadcastBreakContKill(const std::string* name, CharId_t cid, const std::string* breaker_name, CharId_t breaker_cid, int32_t nContLevel)
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    const CfgCampWarTable* campWarTable = CfgData::GetCampWarTable(cfgData);
    const CfgCampWarContKill* pContKill = CfgCampWarTable::GetContKill(campWarTable, nContLevel);
    if (pContKill)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(pContKill->nBreakBroadcast);
            packet->writeUTF8(breaker_name);
            packet->writeInt64(breaker_cid);
            packet->writeUTF8(name);
            packet->writeInt64(cid);
            packet->writeInt32(pContKill->nKillCount);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, 0, packet);
        }
    }
}

int32_t CCampWar::selectCamp(Player* player)
{
    if (!player)
        return 0;
    if (m_Camp_1_battle <= m_Camp_2_battle)
        return 1;
    return 2;
}

int32_t CCampWar::getContTitle(int32_t nContLevel)
{
    if (nContLevel > 0)
    {
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        const CfgCampWarTable* campWarTable = CfgData::GetCampWarTable(cfgData);
        const CfgCampWarContKill* pContKill = CfgCampWarTable::GetContKill(campWarTable, nContLevel);
        if (pContKill)
            return pContKill->nTitle;
    }
    return 0;
}

void CCampWar::notifyAddScore(Player* player, int32_t nPoint, int32_t nKillCount)
{
    if (player)
    {
        int8_t connId = Player::getConnId(player);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E2Bu);
        if (packet)
        {
            packet->writeInt32(nPoint);
            packet->writeInt8(nKillCount);
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = Player::getGateIndex(player);
            int8_t connId2 = Player::getConnId(player);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}
