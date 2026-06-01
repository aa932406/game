#include "Other/CCityWar.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "CTimer.h"
#include "CActivityManager.h"
#include "FamilyManager.h"
#include "StringUtility.h"

CCityWar::CCityWar(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_pMonster = 0;
    m_mFamilyScore.clear();
    m_mPlayerScore.clear();
    m_lFamilyScore.clear();
    m_bUpdateFamilyScroe = 0;
    m_nLastUpdateRankTick = 0;
    m_nFamilyId = 0;
    m_nLastFamilyId = 0;
    m_nWinTime = 0;
    m_Apply.clear();
    m_nIndex = 0;
    m_First = 0;
    m_Second = 0;
    m_Third = 0;
    m_FirstFamilyName.clear();
    m_SecondFamilyName.clear();
    m_ThirdFamilyName.clear();
}

CCityWar::~CCityWar()
{
}

void CCityWar::Init()
{
    CActivity::Init();
    m_nIndex = 0;
    m_nFamilyId = 0;
    m_nLastFamilyId = 0;
    m_nWinTime = 0;
    m_Leader = 0;
    m_First = 0;
    m_Second = 0;
    m_Third = 0;
    m_FirstFamilyName.clear();
    m_SecondFamilyName.clear();
    m_ThirdFamilyName.clear();

    Answer::DBPool* dbPool = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(dbPool);
    Answer::MySqlQuery result(db.query("SELECT * FROM `mem_city_war` ORDER BY `index` DESC LIMIT 1"));

    if (!result.eof())
    {
        m_nIndex = result.getIntValue("index", 0);
        m_nFamilyId = result.getInt64Value("familyid", 0);
        m_nWinTime = result.getIntValue("wintime", 0);
        m_Leader = result.getInt64Value("leader", 0);
        m_First = result.getInt64Value("first", 0);
        m_Second = result.getInt64Value("second", 0);
        m_Third = result.getInt64Value("third", 0);
        m_FirstFamilyName = result.getStringValue("firstfamilyname", "");
        m_SecondFamilyName = result.getStringValue("secondfamilyname", "");
        m_ThirdFamilyName = result.getStringValue("thirdfamilyname", "");
        m_nLastFamilyId = m_nFamilyId;
        CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
        CActivityManager::SetCityWarWinner(am, 0, m_nFamilyId);
    }

    char szSQL[4096];
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, 0xFFFu, "SELECT * FROM `mem_city_war_apply` where `index`=%d", m_nIndex + 1);
    Answer::MySqlQuery result0(db.query(szSQL));
    if (!result0.eof())
    {
        std::string applyInfo = result0.getStringValue("applyinfo", "");
        StringVector vStr;
        std::string delims = "|";
        Answer::StringUtility::split(&vStr, &applyInfo, &delims, 0);
        for (auto& str : vStr)
        {
            StringVector tv;
            std::string delim = ":";
            Answer::StringUtility::split(&tv, &str, &delim, 0);
            if (tv.size() == 2)
            {
                FamilyId_t key = strtoll(tv[0].c_str(), 0, 10);
                m_Apply[key] = atoi(tv[1].c_str());
            }
        }
    }
}

void CCityWar::getFamilyMembers(FamilyId_t nFamilyId, CharIdList* cids)
{
    for (auto& entry : m_mPlayerScore)
    {
        const PlayerScore& score = entry.second;
        if (score.nFamilyId == nFamilyId && score.bInActivity)
            cids->push_back(score.nCharId);
    }
}

void CCityWar::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (pMap && m_nState == ACTIVITY_STATE::AS_RUNNING)
    {
        int64_t nCurTick = Map::getTick(pMap);
        if (nCurTick - m_nLastUpdateRankTick > 999)
        {
            m_nLastUpdateRankTick = nCurTick;
            if (m_bUpdateFamilyScroe)
            {
                m_bUpdateFamilyScroe = 0;
                m_lFamilyScore.sort(FamilyScoreRankGreater());
                checkChangeFamily(1);
                setNeedBroadcastActivityScore();
            }
            for (auto& familyEntry : m_mFamilyScore)
            {
                FamilyScore& score = familyEntry.second;
                CharIdList cids;
                if (score.bUpdateScroeRank)
                {
                    getFamilyMembers(score.nFamilyId, &cids);
                }
                if (score.bUpdateScroeRank)
                {
                    score.bUpdateScroeRank = 0;
                    score.lScoreRank.sort(PlayerScoreRankGreater());
                    GameService* gs = Answer::Singleton<GameService>::instance();
                    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2E2Au);
                    if (!packet)
                        return;
                    appendPlayerRankInfo(packet, &score.lScoreRank);
                    packet->setSize(packet->getWOffset());
                    GameService* gs2 = Answer::Singleton<GameService>::instance();
                    GameService::broadcast(gs2, packet, &cids);
                }
            }
        }
    }
}

void CCityWar::reset()
{
    m_pMonster = 0;
    m_mFamilyScore.clear();
    m_mPlayerScore.clear();
    m_lFamilyScore.clear();
    m_bUpdateFamilyScroe = 0;
    m_nLastUpdateRankTick = 0;
    CActivity::reset();
}

bool CCityWar::isSpecialDay(int32_t nStartDays)
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    if (CfgData::isUniteServer(cfgData))
    {
        if (nStartDays == 2)
            return true;
    }
    else if (nStartDays == 2)
    {
        return true;
    }
    return false;
}

bool CCityWar::isInSpecialDay(int32_t nStartDays)
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    CfgData::isUniteServer(cfgData);
    return nStartDays <= 2;
}

bool CCityWar::checkData()
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    int32_t startDays;
    if (CfgData::isUniteServer(cfgData))
    {
        cfgData = Answer::Singleton<CfgData>::instance();
        startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_UNITE);
    }
    else
    {
        cfgData = Answer::Singleton<CfgData>::instance();
        startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);
    }
    if (isSpecialDay(startDays))
        return true;
    if (isInSpecialDay(startDays))
        return false;
    return CActivity::checkData();
}

bool CCityWar::HaveOwner()
{
    for (auto pPlayer : m_players)
    {
        if (pPlayer && Player::getFamilyId(pPlayer) == m_nFamilyId && Unit::isAlive(pPlayer) && StaticObj::getMapId(pPlayer) == 50130)
            return true;
    }
    return false;
}

void CCityWar::checkChangeFamily(bool isUpdateScore)
{
    if (isUpdateScore)
    {
        auto it = m_mFamilyScore.find(m_nFamilyId);
        if (it != m_mFamilyScore.end() && it->second.nFamilyCamp == 2)
        {
            FamilyId_t familyId0 = GetGuilDerFirstFamily();
            if (familyId0 > 0 && m_nFamilyId != familyId0)
            {
                m_nFamilyId = familyId0;
                GongGao(familyId0, 337);
                setNeedBroadcastActivityScore();
            }
        }
        return;
    }

    int8_t winCamp = 0;
    FamilyId_t familyId = 0;
    for (auto pPlayer : m_players)
    {
        if (!pPlayer || !Unit::isAlive(pPlayer) || StaticObj::getMapId(pPlayer) != 50130)
            continue;
        if (Player::GetCamp(pPlayer) <= 0 || Player::GetCamp(pPlayer) == 4)
            return;
        if (winCamp)
        {
            if (Player::GetCamp(pPlayer) != winCamp)
                break;
        }
        if (winCamp == 3)
        {
            if (Player::getFamilyId(pPlayer) != familyId)
            {
                if (!HaveOwner() && m_nFamilyId > 0)
                    goto LABEL_CLEAR;
                return;
            }
        }
        winCamp = Player::GetCamp(pPlayer);
        familyId = Player::getFamilyId(pPlayer);
    }

    if (winCamp > 0)
    {
        if (winCamp == 2)
        {
            FamilyId_t familyId1 = GetGuilDerFirstFamily();
            if (familyId1 > 0 && m_nFamilyId != familyId1)
            {
                m_nFamilyId = familyId1;
                GongGao(familyId1, 337);
                SetOwner();
                setNeedBroadcastActivityScore();
            }
        }
        else if ((winCamp == 3 || winCamp == 1) && m_nFamilyId != familyId)
        {
            m_nFamilyId = familyId;
            GongGao(familyId, 337);
            SetOwner();
            setNeedBroadcastActivityScore();
        }
    }
    return;

LABEL_CLEAR:
    if (!HaveOwner() && m_nFamilyId > 0)
    {
        m_nFamilyId = 0;
        NotOccupyGongGao();
        SetOwner();
        setNeedBroadcastActivityScore();
    }
}

void CCityWar::NotOccupyGongGao()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(342);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void CCityWar::SetOwner()
{
    for (auto pPlayer : m_players)
    {
        if (pPlayer)
        {
            if (Player::getFamilyId(pPlayer) == m_nFamilyId)
                Player::SetActState(pPlayer, 1);
            else if (Player::GetActState(pPlayer))
                Player::SetActState(pPlayer, 0);
        }
    }
}

FamilyId_t CCityWar::GetGuilDerFirstFamily()
{
    m_lFamilyScore.sort(FamilyScoreRankGreater());
    for (auto pScore : m_lFamilyScore)
    {
        if (pScore->nFamilyCamp == 2)
            return pScore->nFamilyId;
    }
    return 0;
}

bool CCityWar::checkWeek()
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    int32_t startDays;
    if (CfgData::isUniteServer(cfgData))
    {
        cfgData = Answer::Singleton<CfgData>::instance();
        startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_UNITE);
    }
    else
    {
        cfgData = Answer::Singleton<CfgData>::instance();
        startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);
    }
    if (isSpecialDay(startDays))
        return true;
    if (isInSpecialDay(startDays))
        return false;
    return CActivity::checkWeek();
}

void CCityWar::onMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (pMonster && pKiller && IsRuning())
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(336);
            packet->writeInt64(Player::getCid(pKiller));
            std::string val;
            Player::getName(pKiller, &val);
            packet->writeUTF8(&val);
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, packet);
        }
    }
}

void CCityWar::onPlayerKilled(Player* pDier, Player* pAttacker)
{
    if (pDier && pAttacker)
    {
        if (Player::GetCamp(pDier) != 4 && Player::GetCamp(pAttacker) != 4)
        {
            int32_t score = 0;
            int8_t familyPos = Player::getFamilyPosition(pDier);
            if (familyPos == 2 || familyPos == 3)
                score = 200;
            else if (familyPos == 1)
                score = 150;
            else
                score = 100;

            int32_t now = Unit::getNow(pAttacker);
            FamilyId_t familyId = Player::getFamilyId(pAttacker);
            addFamilyScore(familyId, score, now);
            addPlayerScore(pAttacker, score);
        }
        if (StaticObj::getMapId(pDier) == 50130)
            checkChangeFamily(0);

        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(341);
            std::string val;
            Player::getName(pDier, &val);
            packet->writeUTF8(&val);
            packet->writeInt64(Player::getCid(pAttacker));
            packet->writeInt32(StaticObj::getMapId(pAttacker));
            std::string v15;
            Player::getName(pAttacker, &v15);
            packet->writeUTF8(&v15);
            packet->writeInt64(Player::getCid(pDier));
            packet->setSize(packet->getWOffset());
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(gs2, packet);
        }
    }
}

void CCityWar::onMonsterAdd(MonsterActivity* pMonster)
{
    if (pMonster)
    {
        FamilyManager* fm = Answer::Singleton<FamilyManager>::instance();
        FamilyInfo info;
        FamilyManager::GetFamilyInfo(&info, fm, m_nFamilyId);
        std::string familyName = info.FamilyLeaderName;
        Monster::SetFamilyId(pMonster, m_nFamilyId, &familyName);
        m_pMonster = pMonster;
    }
}

int8_t CCityWar::GetFamilyCamp(FamilyId_t familyId)
{
    if (familyId <= 0)
        return 4;
    if (m_nLastFamilyId == familyId)
        return 1;
    auto it = m_Apply.find(familyId);
    if (it == m_Apply.end())
        return 4;
    if (it->second == 1)
        return 2;
    return 3;
}

void CCityWar::addPlayer(Player* player)
{
    if (player)
    {
        FamilyId_t familyId = Player::getFamilyId(player);
        int8_t familyCamp = GetFamilyCamp(familyId);
        Player::SetCamp(player, familyCamp);
        if (Player::getFamilyId(player) > 0)
        {
            if (Player::GetCamp(player) == 2)
                Player::setPkMode(player, 7, 0);
            else
                Player::setPkMode(player, 3, 0);
        }
        else
        {
            Player::setPkMode(player, 6, 0);
        }

        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            iter->second.bInActivity = 1;
            iter->second.nTime = Unit::getNow(player);
        }
        else
        {
            PlayerScore score;
            memset(&score, 0, sizeof(score));
            score.nCharId = Player::getCid(player);
            Player::getName(player, &score.strName);
            score.nFamilyId = Player::getFamilyId(player);
            Player::getFamilyName(player, &score.strFamilyName);
            score.bInActivity = 1;
            score.nFamilyPosition = Player::getFamilyPosition(player);
            score.nTime = Unit::getNow(player);
            m_mPlayerScore[score.nCharId] = score;
            auto pScore = &m_mPlayerScore[score.nCharId];

            if (Player::getFamilyId(player) > 0)
            {
                FamilyId_t fid = Player::getFamilyId(player);
                auto familyIter = m_mFamilyScore.find(fid);
                if (familyIter != m_mFamilyScore.end())
                {
                    familyIter->second.lScoreRank.push_back(pScore);
                }
                else
                {
                    FamilyScore family;
                    memset(&family, 0, sizeof(family));
                    family.nFamilyId = Player::getFamilyId(player);
                    Player::getFamilyName(player, &family.strFamilyName);
                    family.nFamilyCamp = Player::GetCamp(player);
                    family.lScoreRank.push_back(pScore);
                    m_mFamilyScore[family.nFamilyId] = family;
                    m_lFamilyScore.push_back(&m_mFamilyScore[family.nFamilyId]);
                }
            }
        }
        if (Player::getFamilyId(player) == m_nFamilyId)
            Player::SetActState(player, 1);
        CActivity::addPlayer(player);
    }
}

void CCityWar::removePlayer(Player* player, bool islogout)
{
    if (player)
    {
        Player::SetCamp(player, 0);
        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            iter->second.bInActivity = 0;
            iter->second.nSeconds += Unit::getNow(player) - iter->second.nTime;
        }
        CActivity::removePlayer(player, islogout);
        if (m_nState == ACTIVITY_STATE::AS_RUNNING)
            checkChangeFamily(0);
        int8_t connId = Player::getConnId(player);
        CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
        FamilyId_t winFamily = CActivityManager::GetCityWarWinner(am, connId);
        if (winFamily > 0 && Player::getFamilyId(player) == winFamily)
            Player::SetActState(player, 1);
        else if (Player::GetActState(player))
            Player::SetActState(player, 0);
    }
}

bool CCityWar::canGetOnlineReward(PlayerScore* score)
{
    int32_t nOnlineTime = score->nSeconds;
    if (score->bInActivity)
    {
        CTimer* timer = Answer::Singleton<CTimer>::instance();
        nOnlineTime += CTimer::GetNow(timer) - score->nTime;
    }
    return nOnlineTime > 299;
}

void CCityWar::SendPlayerActivityInfo(Player* player)
{
    if (player)
    {
        if (m_nLastFamilyId <= 0)
        {
            int8_t connId = Player::getConnId(player);
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E22u);
            if (!packet)
                return;
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt32(getNextStartTime());
            packet->writeInt32(0);
            packet->writeInt64(m_nLastFamilyId);
            packet->writeUTF8(&m_FirstFamilyName);
            packet->writeUTF8(&m_SecondFamilyName);
            packet->writeUTF8(&m_ThirdFamilyName);
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = Player::getGateIndex(player);
            int8_t connId2 = Player::getConnId(player);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
        else
        {
            int8_t connId = Player::getConnId(player);
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x4EADu);
            if (!packet)
                return;
            packet->writeInt32(Player::getGateIndex(player));
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt32(getNextStartTime());
            packet->writeInt64(m_nLastFamilyId);
            packet->writeInt32(m_nWinTime);
            packet->writeInt64(m_First);
            packet->writeInt64(m_Second);
            packet->writeInt64(m_Third);
            packet->writeUTF8(&m_FirstFamilyName);
            packet->writeUTF8(&m_SecondFamilyName);
            packet->writeUTF8(&m_ThirdFamilyName);
            packet->setSize(packet->getWOffset());
            int8_t connId2 = Player::getConnId(player);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacket(gs2, connId2, packet);
        }
        SendAppyCityWarInfo(player);
    }
}

void CCityWar::SendPlayerActivityScore(Player* player)
{
    if (player)
    {
        int8_t connId = Player::getConnId(player);
        Answer::NetPacket* familyPacket = packetActivityScore(connId);
        if (familyPacket)
        {
            int16_t gateIndex = Player::getGateIndex(player);
            int8_t connId2 = Player::getConnId(player);
            GameService* gs = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs, connId2, gateIndex, familyPacket);
        }
        FamilyId_t fid = Player::getFamilyId(player);
        auto iter = m_mFamilyScore.find(fid);
        if (iter != m_mFamilyScore.end())
        {
            GameService* gs = Answer::Singleton<GameService>::instance();
            Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2E2Au);
            if (packet)
            {
                appendPlayerRankInfo(packet, &iter->second.lScoreRank);
                packet->setSize(packet->getWOffset());
                int16_t gateIndex2 = Player::getGateIndex(player);
                int8_t connId3 = Player::getConnId(player);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                GameService::sendPacketTo(gs2, connId3, gateIndex2, packet);
            }
        }
    }
}

Position CCityWar::GetRandBornPos(Player* player)
{
    Position pos;
    if (player)
    {
        if (m_nFamilyId > 0 && Player::getFamilyId(player) == m_nFamilyId)
            return CActivity::getBornRandPosB(this);
        else
            return CActivity::getBornRandPosA(this);
    }
    pos.x = -1;
    pos.y = -1;
    return pos;
}

int32_t CCityWar::GetRevive(Player* player)
{
    if (!player)
        return CActivity::GetRevive(this, 0);
    if (m_nFamilyId > 0 && Player::getFamilyId(player) == m_nFamilyId)
    {
        int32_t len = m_cfgActivity->target_regionb.size();
        if (len == 1)
            return m_cfgActivity->target_regionb[0];
        Answer::Random* rnd = Answer::Singleton<Answer::Random>::instance();
        int32_t idx = Answer::Random::generate(rnd, 0, len - 1);
        return m_cfgActivity->target_regionb[idx];
    }
    else
    {
        int32_t len = m_cfgActivity->target_regiona.size();
        if (len == 1)
            return m_cfgActivity->target_regiona[0];
        Answer::Random* rnd = Answer::Singleton<Answer::Random>::instance();
        int32_t idx = Answer::Random::generate(rnd, 0, len - 1);
        return m_cfgActivity->target_regiona[idx];
    }
}

void CCityWar::addPlayerScore(Player* player, int32_t nScore)
{
    if (player)
    {
        auto iter = m_mPlayerScore.find(Player::getCid(player));
        if (iter != m_mPlayerScore.end())
        {
            iter->second.nScore += nScore;
            iter->second.nScoreTime = Unit::getNow(player);
            setUpdateScoreRank(Player::getFamilyId(player), nScore > 0);
        }
    }
}

void CCityWar::setUpdateScoreRank(FamilyId_t nFamilyId, bool bScoreRank)
{
    auto iter = m_mFamilyScore.find(nFamilyId);
    if (iter != m_mFamilyScore.end() && bScoreRank)
        iter->second.bUpdateScroeRank = 1;
}

void CCityWar::addFamilyScore(FamilyId_t nFamilyId, int32_t nScore, int32_t nNowTime)
{
    auto iter = m_mFamilyScore.find(nFamilyId);
    if (iter != m_mFamilyScore.end())
    {
        iter->second.nScore += nScore;
        iter->second.nScoreTime = nNowTime;
        m_bUpdateFamilyScroe = 1;
        setNeedBroadcastActivityScore();
    }
}

void CCityWar::win()
{
    if (m_nFamilyId != m_nLastFamilyId)
    {
        m_nWinTime = Answer::DayTime::now();
        FamilyId_t oldFamily = m_nLastFamilyId;
        FamilyId_t newFamily = m_nFamilyId;
        GameService* gs = Answer::Singleton<GameService>::instance();
        GameService::UpdateCityWarTitle(gs, oldFamily, newFamily);
        m_nLastFamilyId = m_nFamilyId;
    }

    CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetCityWarWinner(am, 0, m_nFamilyId);

    if (m_nFamilyId > 0)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        GameService::UpdateCityActState(gs, m_nFamilyId, 1);

        FamilyManager* fm = Answer::Singleton<FamilyManager>::instance();
        FamilyInfo info;
        FamilyManager::GetFamilyInfo(&info, fm, m_nFamilyId);
        if (FamilyInfo::IsEmpty(&info))
            return;

        if (m_nIndex == 1)
        {
            CfgData* cfgData = Answer::Singleton<CfgData>::instance();
            if (CfgData::isUniteServer(cfgData))
            {
                DBService* db = Answer::Singleton<DBService>::instance();
                DBService::OnSendSysMail(db, 0, info.FamilyLeaderCid, 6397, nullptr, 0);
            }
            else
            {
                DBService* db = Answer::Singleton<DBService>::instance();
                DBService::OnSendSysMail(db, 0, info.FamilyLeaderCid, 6371, nullptr, 0);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(gs2, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
                if (!packet)
                    return;
                packet->writeInt32(442);
                packet->writeInt64(info.FamilyLeaderCid);
                packet->writeUTF8(&info.FamilyLeaderName);
                packet->setSize(packet->getWOffset());
                GameService* gs3 = Answer::Singleton<GameService>::instance();
                GameService::worldBroadcast(gs3, packet);
            }
        }

        GameService* gs4 = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet2 = GameService::popNetpacket(gs4, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (!packet2)
            return;
        packet2->writeInt32(338);
        packet2->writeUTF8(&info.strName);
        packet2->setSize(packet2->getWOffset());
        GameService* gs5 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs5, packet2);
    }
    else
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (!packet)
            return;
        packet->writeInt32(339);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }

    m_nState = ACTIVITY_STATE::AS_END;
    addRewards();
    saveResult();
    OnActivityEnd();
    BroadcastActivityState();
    delayKickAll(30);
    OnActivityFinish();
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::broadFamilyWarIcon(gs);
}

void CCityWar::addRewards()
{
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    const CfgCityWarTable* cityWarTable = CfgData::GetCityWarTable(cfgData);
    const CfgCityWarContReward* joinReward = CfgCityWarTable::GetContReward(cityWarTable, 4);
    if (!joinReward)
        return;

    for (auto& entry : m_mPlayerScore)
    {
        PlayerScore& score = entry.second;
        if (canGetOnlineReward(&score))
        {
            DBService* db = Answer::Singleton<DBService>::instance();
            db->OnSendSysMail(0, score.nCharId, joinReward->nBreakMailId, &joinReward->vBreakReward, ITEM_CHANGE_REASON::ICR_CITY_WAR_REWARD, nullptr, 0);
        }
    }

    int32_t otherFamilyIndex = 2;
    int32_t familyIndex = 0;
    for (auto pFamilyScore : m_lFamilyScore)
    {
        if (pFamilyScore->nFamilyId == m_nFamilyId)
            familyIndex = 1;
        else
            familyIndex = otherFamilyIndex++;

        if (familyIndex > 3 || (pFamilyScore->nScore <= 0 && familyIndex != 1))
            continue;

        if (familyIndex == 2)
            m_FirstFamilyName = pFamilyScore->strFamilyName;
        else if (familyIndex == 3)
            m_SecondFamilyName = pFamilyScore->strFamilyName;

        int32_t otherPlayerIndex = 2;
        int32_t playerIndex = 0;
        const CfgCityWarContReward* pCfgContReward = CfgCityWarTable::GetContReward(cityWarTable, familyIndex);
        if (!pCfgContReward)
            continue;

        for (auto pPlayerScore : pFamilyScore->lScoreRank)
        {
            if (pPlayerScore->nFamilyPosition == 3)
                playerIndex = 1;
            else
                playerIndex = otherPlayerIndex++;

            if (playerIndex > 6 || (pPlayerScore->nScore <= 0 && playerIndex != 1))
                continue;

            int32_t rewardIndex = 0;
            switch (playerIndex)
            {
                case 1: rewardIndex = 1; break;
                case 2:
                    if (familyIndex == 1) m_First = pPlayerScore->nCharId;
                    rewardIndex = 2; break;
                case 3:
                    if (familyIndex == 1) m_Second = pPlayerScore->nCharId;
                    rewardIndex = 3; break;
                case 4:
                    if (familyIndex == 1) m_Third = pPlayerScore->nCharId;
                    rewardIndex = 3; break;
                case 5:
                case 6: rewardIndex = 4; break;
                default: break;
            }

            if (rewardIndex > 0)
            {
                DBService* db = Answer::Singleton<DBService>::instance();
                db->OnSendSysMail(0, pPlayerScore->nCharId, pCfgContReward->nBreakMailId, &pCfgContReward->vBreakReward[rewardIndex - 1], ITEM_CHANGE_REASON::ICR_CITY_WAR_REWARD, nullptr, 0);
            }
        }
    }
}

void CCityWar::saveResult()
{
    int32_t id = GetId();
    DBService* db = Answer::Singleton<DBService>::instance();
    DBService::SaveCityWarResult(db, id, m_nFamilyId, m_nWinTime, m_nStartTime, m_Leader,
        m_First, m_Second, m_Third, m_nIndex, &m_FirstFamilyName, &m_SecondFamilyName, &m_ThirdFamilyName);
}

void CCityWar::onActivityStart()
{
    ++m_nIndex;
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::UpdateCityWarTitle(gs, m_nLastFamilyId, 0);
    CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetCityWarWinner(am, 0, 0);
    sendSocialUpdateActivityState(1);
    gs = Answer::Singleton<GameService>::instance();
    GameService::UpdateCityActState(gs, m_nFamilyId, 0);
    m_nFamilyId = m_nLastFamilyId;
    AutoApplyCityWar();
}

void CCityWar::onTimeEnd()
{
    win();
    m_Apply.clear();
    CActivity::onTimeEnd();
    sendSocialUpdateActivityState(0);
}

int32_t CCityWar::getNextStartTime()
{
    if (IsRuning() || m_cfgActivity->start_hour.empty())
        return 0;

    CTimer* timer = Answer::Singleton<CTimer>::instance();
    int32_t nNowTime = CTimer::GetNow(timer);
    timer = Answer::Singleton<CTimer>::instance();
    tm localnow;
    CTimer::GetLocalNow(&localnow, timer);
    int32_t nowMinute = 60 * localnow.tm_hour + localnow.tm_min;
    int32_t startMinute = m_cfgActivity->start_hour[0];

    for (uint32_t i = 0; i < m_cfgActivity->start_hour.size(); ++i)
    {
        int32_t v5 = Answer::DayTime::dayzero(nNowTime);
        int32_t startTime = v5 + 60 * m_cfgActivity->start_hour[i];
        if (startMinute > nNowTime)
        {
            startMinute = m_cfgActivity->start_hour[i];
            break;
        }
    }

    int32_t days = -1;
    int32_t startDays = 0;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    if (CfgData::isUniteServer(cfgData))
        startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_UNITE);
    else
        startDays = CfgData::getServerDiffDay(cfgData, SERVER_TYPE::SVT_NORMAL);

    int32_t specialDay = 2;
    if (IsRightLine() && checkData())
    {
        if (nowMinute < startMinute)
            days = 0;
    }
    else if (isInSpecialDay(startDays) && startDays < specialDay)
    {
        days = specialDay - startDays;
    }

    if (days < 0)
    {
        if (m_cfgActivity->weekday.empty())
            return 0;
        timer = Answer::Singleton<CTimer>::instance();
        int32_t weekday = CTimer::GetWeekDay(timer);
        int32_t nextweekday = 0;

        for (auto rit = m_cfgActivity->weekday.rbegin(); rit != m_cfgActivity->weekday.rend(); ++rit)
        {
            if (*rit > weekday)
            {
                nextweekday = *rit;
                break;
            }
        }
        if (nextweekday == 0)
            nextweekday = m_cfgActivity->weekday.back();

        if (nextweekday >= weekday)
            days = nextweekday - weekday;
        else
            days = nextweekday + 7 - weekday;
    }

    return 86400 * days + 60 * startMinute + Answer::DayTime::dayzero(nNowTime);
}

void CCityWar::SendAppyCityWarInfo(Player* player)
{
    if (player)
    {
        int8_t connId = Player::getConnId(player);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E2Eu);
        if (packet)
        {
            packet->writeInt32(m_Apply.size());
            for (auto& entry : m_Apply)
            {
                packet->writeInt64(entry.first);
                FamilyManager* fm = Answer::Singleton<FamilyManager>::instance();
                FamilyInfo info;
                FamilyManager::GetFamilyInfo(&info, fm, entry.first);
                packet->writeUTF8(&info.strName);
                packet->writeInt32(entry.second);
            }
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = Player::getGateIndex(player);
            int8_t connId2 = Player::getConnId(player);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}

bool CCityWar::ApplyCityWar(Player* player, FamilyId_t familyId, int8_t type)
{
    if (!player)
        return false;
    if (m_nLastFamilyId == familyId)
        return false;

    auto it = m_Apply.find(familyId);
    if (it != m_Apply.end())
    {
        if (type == 2)
        {
            m_Apply[familyId] = 1;
            SendAppyCityWarInfo(player);
            SaveApplyInfo();
            return true;
        }
        else if (type == 3)
        {
            m_Apply[familyId] = Unit::getNow(player);
            SendAppyCityWarInfo(player);
            SaveApplyInfo();
            return true;
        }
        return false;
    }
    else
    {
        if (type == 1)
        {
            m_Apply[familyId] = 0;
            SendAppyCityWarInfo(player);
            SaveApplyInfo();
            GongGao(familyId, 340);
            return true;
        }
        return false;
    }
}

void CCityWar::AutoApplyCityWar()
{
    FamilyManager* fm = Answer::Singleton<FamilyManager>::instance();
    Int64List familyIdList;
    FamilyManager::GetTop10Family(fm, &familyIdList);
    for (auto familyId : familyIdList)
    {
        auto it = m_Apply.find(familyId);
        if (it == m_Apply.end())
        {
            m_Apply[familyId] = 0;
            GongGao(familyId, 340);
        }
    }
    SaveApplyInfo();
}

void CCityWar::GongGao(FamilyId_t familyId, int32_t gongGaoId)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(gongGaoId);
        packet->writeInt64(familyId);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void CCityWar::SaveApplyInfo()
{
    DBService* db = Answer::Singleton<DBService>::instance();
    std::string applyInfo;
    for (auto& entry : m_Apply)
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "%lld:%d|", entry.first, entry.second);
        applyInfo += buf;
    }
    DBService::SaveCityWarApplyInfo(db, m_nIndex + 1, &applyInfo);
}

int32_t CCityWar::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (player && pTargetMap)
        return CActivity::canEnter(player, pTargetMap);
    return 10002;
}

void CCityWar::packetActivityScore(int8_t connid)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connid, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (!packet)
        return;
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt32(getLeftTime());
    packet->writeInt64(m_nFamilyId);
    packet->writeUTF8(&m_FirstFamilyName);
    packet->writeUTF8(&m_SecondFamilyName);
    packet->writeUTF8(&m_ThirdFamilyName);
    packet->setSize(packet->getWOffset());
}

void CCityWar::appendPlayerRankInfo(Answer::NetPacket* packet, std::list<PlayerScore*>* scoreList)
{
    if (!packet)
        return;
    int32_t id = GetId();
    packet->writeInt32(id);
    int32_t nCount = 0;
    uint32_t oldOffset = packet->getWOffset();
    packet->writeInt32(0);
    for (auto iter = scoreList->begin(); iter != scoreList->end(); ++iter)
    {
        if (iter == scoreList->end())
            break;
        PlayerScore* pScore = *iter;
        if (!pScore || pScore->nScore <= 0)
            break;
        packet->writeInt64(pScore->nCharId);
        packet->writeUTF8(&pScore->strName);
        packet->writeInt32(pScore->nScore);
        ++nCount;
    }
    if (nCount)
    {
        uint32_t newOffset = packet->getWOffset();
        packet->setWOffset(oldOffset);
        packet->writeInt32(nCount);
        packet->setWOffset(newOffset);
    }
}

void CCityWar::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(330);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void CCityWar::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(331);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}
