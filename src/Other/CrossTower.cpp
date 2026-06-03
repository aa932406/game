#include "common.h"
#include "Other/CrossTower.h"
#include "Answer.h"
#include "GameService.h"
#include "CfgData.h"
#include "MapManager.h"
#include "CTimer.h"
#include "Player.h"
#include "Unit.h"
#include "CActivityManager.h"
#include "DBService.h"
#include "CExtCharBag.h"
#include "CExtOperateLimit.h"
#include "Monster.h"
#include "Plant.h"
#include "Map.h"
#include "ItemChangeReason.h"

CrossTower::CrossTower(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
    m_ConnId = 0;
    m_LastAddScoreTime = 0;
    m_LastUpdateTime = 0;
    m_NewWinerId = 0;
    m_State = 0;
    m_StateTime = 0;
    m_WinerId = 0;
    m_Battle = 0;
    m_Monster = 0;
    m_NeedChangeMap = 0;
    m_NeedAddScore = 0;
}

CrossTower::~CrossTower()
{
}

void CrossTower::Init()
{
    CActivity::Init();
    m_WinerId = 0;
    m_WinerName.clear();
    m_Monster = 0;
    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 9)
    {
        DBService* db = Answer::Singleton<DBService>::instance();
        DBService::onLoadCrossTowerInfo(db);
    }
    else
    {
        Answer::DBPool* dbPool = Answer::Singleton<Answer::DBPool>::instance();
        Answer::MySqlDBGuard db(dbPool);
        Answer::MySqlQuery* q = Answer::MySqlDBGuard::query(&db, "SELECT * FROM `mem_cross_tower` where id=1");
        Answer::MySqlQuery result(q);
        if (!Answer::MySqlQuery::eof(&result))
        {
            m_WinerId = Answer::MySqlQuery::getInt64Value(&result, "cid", 0);
            const char* name = Answer::MySqlQuery::getStringValue(&result, "name", "");
            m_WinerName = name;
            int32_t battle = Answer::MySqlQuery::getIntValue(&result, "battle", 0);
            if (m_WinerId > 0)
            {
                CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
                CActivityManager::SetCrossTowerWinner(am, m_WinerId, battle, &m_WinerName);
            }
        }
    }
}

void CrossTower::OnUpdate(CActivityMap* pMap)
{
    CActivity::OnUpdate(pMap);
    if (!pMap)
        return;
    auto it = m_NeedSysMap.find(Map::GetMapId(pMap));
    if (it != m_NeedSysMap.end())
    {
        Answer::NetPacket* packet = packetActivityMapInfo(pMap);
        if (!packet)
            return;
        Map::broadcast(pMap, packet);
        m_NeedSysMap.erase(it);
    }
    if (m_NeedChangeMap)
    {
        PlayerList tList(m_players);
        for (auto player : tList)
        {
            int32_t mapId = StaticObj::getMapId(player);
            CfgData* cfgData = Answer::Singleton<CfgData>::instance();
            CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, mapId);
            if (pCfg && (m_State != 3 || pCfg->Floor != 8) && pCfg->NextMapId > 0)
            {
                int32_t record = Player::getRecord(player, 3);
                if (record >= pCfg->KillCount)
                {
                    int32_t nextMapId = pCfg->NextMapId;
                    MapManager* mm = Answer::Singleton<MapManager>::instance();
                    Map* pTargetMap = MapManager::GetMap(mm, nextMapId);
                    if (pTargetMap)
                    {
                        int32_t reviveId = Map::GetReviveRegion(pTargetMap);
                        CfgData* cfgData2 = Answer::Singleton<CfgData>::instance();
                        const CfgMapRegion* pRegion = CfgData::getMapRegion(cfgData2, reviveId);
                        if (pRegion)
                        {
                            Position pos = Map::getRandomWalkablePositionInRegion(pTargetMap, pRegion);
                            if (pos.x < 0 || pos.y < 0)
                            {
                                pos.x = 32;
                                pos.y = 56;
                            }
                            Player::FullHPAndSync(player);
                            Player::switchMap(player, pTargetMap, pos.x, pos.y, 0);
                        }
                    }
                }
            }
        }
        m_NeedChangeMap = 0;
    }
    if (m_NeedAddScore)
    {
        PlayerList tList0(m_players);
        for (auto player : tList0)
        {
            int32_t mapId = StaticObj::getMapId(player);
            CfgData* cfgData = Answer::Singleton<CfgData>::instance();
            CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, mapId);
            Map* pMapObj = StaticObj::getMap(player);
            if (pCfg && pMapObj)
            {
                int32_t floor = pCfg->Floor;
                int32_t aliveSize = Map::GetMapPlayerAliveSize(pMapObj);
                int32_t addScore = CallAddScore(pCfg->MaxPlayer, pCfg->MaxScore, pCfg->MinScore, aliveSize, floor);
                if (addScore > 0)
                {
                    Player::AddCurrency(player, CURRENCY_TYPE::CURRENCY_DUST, addScore, CURRENCY_CHANGE_REASON::GCR_CROSS_TOWER, 0);
                    CharId_t cid = Player::getCid(player);
                    auto scoreIt = m_mPlayerScore.find(cid);
                    if (scoreIt != m_mPlayerScore.end())
                    {
                        scoreIt->second.nScore += addScore;
                        scoreIt->second.nScoreTime = Unit::getNow(player);
                        sendPlayerScore(player);
                    }
                }
            }
        }
        m_NeedAddScore = 0;
    }
    if (Map::GetMapId(pMap) != 60029)
        CheckWin(false);
}

Answer::NetPacket* CrossTower::packetActivityMapInfo(CActivityMap* pMap)
{
    if (!pMap)
        return 0;
    int32_t mapId = Map::GetMapId(pMap);
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, mapId);
    if (!pCfg)
        return 0;
    int32_t floor = pCfg->Floor;
    int32_t aliveSize = Map::GetMapPlayerAliveSize(pMap);
    int32_t addScore = CallAddScore(pCfg->MaxPlayer, pCfg->MaxScore, pCfg->MinScore, aliveSize, floor);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2E31u);
    if (!packet)
        return 0;
    int32_t v8 = Map::GetMapPlayerAliveSize(pMap);
    packet->writeInt32(v8);
    packet->writeInt32(addScore);
    packet->setSize(packet->getWOffset());
    return packet;
}

int32_t CrossTower::onBeginGather(PlantActivity* plant, Player* player)
{
    if (!plant || !player)
        return 10002;
    if (Player::getRecord(player, 2110) <= 3)
        return 0;
    return 10002;
}

void CrossTower::onPlantGather(PlantActivity* pPlant, Player* player)
{
    if (pPlant && player)
    {
        CExtOperateLimit* limit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(limit, 2110, 1);
    }
}

Position CrossTower::GetRandBornPos(Player* player)
{
    if (!player)
        return CActivity::GetRandBornPos(nullptr);
    int32_t enterMapId = GetEnterMapId(player);
    MapManager* mm = Answer::Singleton<MapManager>::instance();
    Map* pTargetMap = MapManager::GetMap(mm, enterMapId);
    if (pTargetMap)
    {
        int32_t reviveId = Map::GetReviveRegion(pTargetMap);
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        const CfgMapRegion* pRegion = CfgData::getMapRegion(cfgData, reviveId);
        if (pRegion)
            return Map::getRandomWalkablePositionInRegion(pTargetMap, pRegion);
    }
    return CActivity::GetRandBornPos(player);
}

void CrossTower::reset()
{
    m_State = 0;
    m_StateTime = 0;
    m_LastUpdateTime = 0;
    m_mPlayerScore.clear();
    m_NeedChangeMap = 0;
    m_NeedAddScore = 0;
    m_LastAddScoreTime = 0;
    m_NeedSysMap.clear();
    m_NewWinerId = 0;
    m_NewWinerName.clear();
    m_ConnId = 0;
    m_Battle = 0;
    m_Monster = 0;
    CActivity::reset();
}

int32_t CrossTower::canEnter(Player* player, CActivityMap* pTargetMap)
{
    if (player && pTargetMap)
        return CActivity::canEnter(player, pTargetMap);
    return 10002;
}

void CrossTower::addPlayer(Player* player)
{
    if (!player)
        return;
    if (m_State == 1)
        Player::setPkMode(player, 1, 0);
    else
        Player::setPkMode(player, 6, 0);
    m_NeedSysMap.insert(StaticObj::getMapId(player));
    Player::updateRecord(player, 3, 0);
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it != m_mPlayerScore.end())
    {
        it->second.bInActivity = 1;
        it->second.nCurMapId = StaticObj::getMapId(player);
        it->second.nBattle = Player::getBattle(player);
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, it->second.nCurMapId);
        CfgData* cfgData2 = Answer::Singleton<CfgData>::instance();
        CrossTowerCfg* pMaxCfg = CfgData::GetCrossTowerCfg(cfgData2, it->second.nMaxMapId);
        if (pCfg && pMaxCfg && pCfg->Floor > pMaxCfg->Floor)
            it->second.nMaxMapId = it->second.nCurMapId;
    }
    else
    {
        PlayerScore score;
        memset(&score, 0, sizeof(score));
        score.nCharId = Player::getCid(player);
        Player::getName(player, &score.strName);
        score.bInActivity = 1;
        score.nConnId = Player::getConnId(player);
        score.nCurMapId = StaticObj::getMapId(player);
        score.nMaxMapId = StaticObj::getMapId(player);
        score.nBattle = Player::getBattle(player);
        score.nSid = Player::getSid(player);
        score.nEnterTime = Unit::getTick(player);
        m_mPlayerScore[score.nCharId] = score;
    }
    CActivity::addPlayer(player);
}

void CrossTower::removePlayer(Player* player, bool islogout)
{
    CActivity::removePlayer(player, islogout);
    if (player)
    {
        m_NeedSysMap.insert(StaticObj::getMapId(player));
        Player::updateRecord(player, 3, 0);
        CharId_t cid = Player::getCid(player);
        auto it = m_mPlayerScore.find(cid);
        if (it != m_mPlayerScore.end())
            it->second.bInActivity = 0;
    }
}

void CrossTower::broadcastReady()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(511);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void CrossTower::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(512);
        packet->writeInt32(GetId());
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

void CrossTower::onTimeEnd()
{
    CheckWin(true);
    addRewards();
    m_nState = ACTIVITY_STATE::AS_END;
    delayKickAll(30);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(513);
        packet->writeInt32(GetId());
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, packet);
    }
}

int32_t CrossTower::GiveDailyReward(Player* player)
{
    if (!player)
        return 10002;
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it == m_mPlayerScore.end())
        return 10002;
    if (it->second.nIsMoBai > 0)
        return 10002;
    it->second.nIsMoBai = 1;
    Player::AddCurrency(player, CURRENCY_TYPE::CURRENCY_DUST, 10000, CURRENCY_CHANGE_REASON::GCR_CROSS_TOWER, 0);
    sendPlayerScore(player);
    return 0;
}

void CrossTower::addRewards()
{
    if (m_WinerId > 0)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        GameService::onRemoveTitle(gs, m_WinerId, 9);
    }
    m_WinerId = m_NewWinerId;
    m_WinerName = m_NewWinerName;
    if (m_NewWinerId > 0)
    {
        GameService* gs = Answer::Singleton<GameService>::instance();
        GameService::onCheckTitle(gs, m_NewWinerId, 9, 1);
    }
    CActivityManager* am = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetCrossTowerResult(am, m_NewWinerId, m_Battle, &m_NewWinerName);
    CTimer* timer = Answer::Singleton<CTimer>::instance();
    int32_t time = CTimer::GetNow(timer);
    int32_t id = GetId();
    DBService* db = Answer::Singleton<DBService>::instance();
    db->SaveCrossTowerResult(id, m_NewWinerId, &m_NewWinerName, m_Battle, m_ConnId, time);
    broadcastActivityResult();
    for (auto& entry : m_mPlayerScore)
    {
        const PlayerScore* score = &entry.second;
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, score->nCurMapId);
        if (pCfg)
        {
            if (score->bInActivity)
            {
                db = Answer::Singleton<DBService>::instance();
                db->OnSendSysMail(score->nConnId, score->nCharId, pCfg->MailId, &pCfg->RewardVt, ITEM_CHANGE_REASON::ICR_CROSS_TOWER, nullptr, 0);
            }
            else
            {
                db = Answer::Singleton<DBService>::instance();
                db->OnSendSysMail(score->nConnId, score->nCharId, 6408, nullptr, 0);
            }
        }
    }
}

void CrossTower::broadcastActivityResult()
{
    for (auto& entry : m_mPlayerScore)
    {
        PlayerScore* score = &entry.second;
        if (score->bInActivity)
        {
            GameService* gs = Answer::Singleton<GameService>::instance();
            Player* player = GameService::getPlayer(gs, score->nCharId, 0, 0);
            if (player)
            {
                int8_t connId = Player::getConnId(player);
                GameService* gs2 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket* packet = GameService::popNetpacket(gs2, connId, Answer::PackType::PACK_DISPATCH, 0x2E27u);
                if (!packet)
                    return;
                packet->writeInt32(m_cfgActivity->id);
                packet->writeInt32(score->nCurMapId);
                packet->writeUTF8(&m_NewWinerName);
                packet->setSize(packet->getWOffset());
                int16_t gateIndex = Player::getGateIndex(player);
                int8_t connId2 = Player::getConnId(player);
                GameService* gs3 = Answer::Singleton<GameService>::instance();
                GameService::sendPacketTo(gs3, connId2, gateIndex, packet);
            }
        }
    }
}

void CrossTower::sendPlayerScore(Player* player)
{
    if (!player)
        return;
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it != m_mPlayerScore.end())
    {
        int8_t connId = Player::getConnId(player);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E24u);
        if (packet)
        {
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt32(it->second.nScore);
            packet->writeInt32(Player::getRecord(player, 3));
            packet->writeInt8(it->second.nIsMoBai);
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = Player::getGateIndex(player);
            int8_t connId2 = Player::getConnId(player);
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}

void CrossTower::onPlayerKilled(Player* pDier, Player* pKiller)
{
    if (pDier && pKiller)
    {
        m_NeedSysMap.insert(StaticObj::getMapId(pDier));
        int32_t mapId = StaticObj::getMapId(pKiller);
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, mapId);
        if (pCfg)
        {
            int32_t killCount = pCfg->KillCount;
            if (killCount > Player::getRecord(pKiller, 3))
            {
                CExtOperateLimit* limit = Player::GetOperateLimit(pKiller);
                CExtOperateLimit::AddLimitCount(limit, 3, 1);
                Player::SetNeedSyncAround(pKiller);
            }
            sendPlayerScore(pKiller);
            CharId_t cid = Player::getCid(pKiller);
            auto it = m_mPlayerScore.find(cid);
            if (it != m_mPlayerScore.end())
            {
                bool gongGao = false;
                ++it->second.nKillCount;
                int32_t nKillCount = it->second.nKillCount;
                if (nKillCount == 30 || nKillCount == 5 || nKillCount == 15 || nKillCount == 100 || nKillCount == 150 || nKillCount == 50)
                    gongGao = true;
                if (it->second.nKillCount > 150 && it->second.nKillCount % 50 == 0)
                    gongGao = true;
                if (gongGao)
                {
                    int8_t connId = Player::getConnId(pKiller);
                    GameService* gs = Answer::Singleton<GameService>::instance();
                    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
                    if (packet)
                    {
                        packet->writeInt32(514);
                        std::string name;
                        Player::getName(pKiller, &name);
                        packet->writeUTF8(&name);
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
}

void CrossTower::packetActivityScore(int8_t connid)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, Answer::PackType::PACK_DISPATCH, 0x2E26u);
    if (!packet)
        return;
    int32_t diffTime;
    if (m_State == 1)
        diffTime = 120;
    else
        diffTime = 60;
    packet->writeInt32(m_cfgActivity->id);
    packet->writeInt8(m_State);
    packet->writeInt32(diffTime + m_StateTime);
    if (m_LastAddScoreTime <= 0)
        packet->writeInt32(m_LastAddScoreTime);
    else
        packet->writeInt32(m_LastAddScoreTime + 20);
    packet->writeInt32(getLeftTime());
    packet->writeInt64(m_WinerId);
    packet->setSize(packet->getWOffset());
}

void CrossTower::onMonsterAdd(MonsterActivity* pMonster)
{
    if (pMonster && Monster::getMid(pMonster) == 2033)
    {
        Monster::SetFamilyId(pMonster, 0, &m_WinerName);
        m_Monster = pMonster;
    }
}

void CrossTower::onMonsterDie(MonsterActivity* pMonster, Player* pKiller)
{
    if (pKiller && pMonster)
    {
        int32_t mapId = StaticObj::getMapId(pKiller);
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, mapId);
        if (pCfg)
        {
            int32_t killCount = pCfg->KillCount;
            if (killCount > Player::getRecord(pKiller, 3))
            {
                CExtOperateLimit* limit = Player::GetOperateLimit(pKiller);
                CExtOperateLimit::AddLimitCount(limit, 3, 1);
                Player::SetNeedSyncAround(pKiller);
                sendPlayerScore(pKiller);
            }
        }
    }
}

void CrossTower::ActUpdate(int64_t curTick)
{
    CTimer* timer = Answer::Singleton<CTimer>::instance();
    int32_t time = CTimer::GetNow(timer);
    m_LastUpdateTime = curTick;
    if (m_LastAddScoreTime > 0 && time - m_LastAddScoreTime > 19)
    {
        m_NeedAddScore = 1;
        m_LastAddScoreTime = time;
        setNeedBroadcastActivityScore();
    }
    if (m_State == 1)
    {
        if (getLeftTime() > 300)
        {
            if (time - m_StateTime > 119)
            {
                PlayerList tList(m_players);
                for (auto player : tList)
                    Player::setPkMode(player, 6, 0);
                m_State = 2;
                m_StateTime = time;
                m_LastAddScoreTime = time;
                setNeedBroadcastActivityScore();
            }
        }
        else
        {
            m_State = 3;
            m_LastAddScoreTime = time;
            setNeedBroadcastActivityScore();
        }
    }
    else if (m_State == 0)
    {
        m_State = 1;
        m_StateTime = time;
        setNeedBroadcastActivityScore();
    }
    else if (m_State == 2)
    {
        if (getLeftTime() <= 300)
        {
            m_State = 3;
            setNeedBroadcastActivityScore();
        }
        if (time - m_StateTime > 59)
        {
            m_NeedChangeMap = 1;
            m_StateTime = time;
            setNeedBroadcastActivityScore();
        }
    }
    else if (m_State == 3 && time - m_StateTime > 59)
    {
        m_NeedChangeMap = 1;
        m_StateTime = time;
        setNeedBroadcastActivityScore();
    }
}

int32_t CrossTower::GetEnterMapId(Player* player)
{
    if (!player)
        return CActivity::GetEnterMapId(nullptr);
    CharId_t cid = Player::getCid(player);
    auto it = m_mPlayerScore.find(cid);
    if (it == m_mPlayerScore.end())
        return CActivity::GetEnterMapId(player);
    int32_t nCurMapId = it->second.nCurMapId;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, nCurMapId);
    if (!pCfg)
        return CActivity::GetEnterMapId(player);
    if (pCfg->LastMapId > 0)
        return pCfg->LastMapId;
    return it->second.nCurMapId;
}

int32_t CrossTower::GetRevive(Player* player)
{
    if (!player)
        return CActivity::GetRevive(nullptr);
    int32_t mapId = StaticObj::getMapId(player);
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    CrossTowerCfg* pCfg = CfgData::GetCrossTowerCfg(cfgData, mapId);
    if (!pCfg)
        return CActivity::GetRevive(player);
    int32_t targetMapId = pCfg->LastMapId;
    if (pCfg->Floor <= 5)
        targetMapId = pCfg->MapId;
    MapManager* mm = Answer::Singleton<MapManager>::instance();
    Map* pTargetMap = MapManager::GetMap(mm, targetMapId);
    if (pTargetMap)
        return Map::GetReviveRegion(pTargetMap);
    return CActivity::GetRevive(player);
}

void CrossTower::onPlayerRevive(Player* player, bool bSafe)
{
    if (player)
        m_NeedSysMap.insert(StaticObj::getMapId(player));
}

int32_t CrossTower::CallAddScore(int32_t MaxPlayer, int32_t MaxScore, int32_t MinScore, int32_t CurPlayer, int32_t Floor)
{
    if (CurPlayer <= MaxPlayer)
        return MaxScore;
    int32_t diffCount = CurPlayer - MaxPlayer;
    int32_t getScore;
    if (Floor <= 8)
        getScore = (int)((float)((float)(15 * diffCount) / -100.0f + 1.0f) * (float)MaxScore);
    else
        getScore = (int)(pow(0.5, (float)diffCount) * (float)MaxScore);
    if (getScore < MinScore)
        return MinScore;
    return getScore;
}

void CrossTower::CheckWin(bool isTimeEnd)
{
    if (m_NewWinerId > 0 || m_State != 3)
        return;
    PlayerScoreSet scoreRank;
    for (auto& entry : m_mPlayerScore)
    {
        if (entry.second.nCurMapId == 60029 && entry.second.bInActivity)
            scoreRank.insert(&entry.second);
    }
    if (!scoreRank.empty())
    {
        auto randIt = scoreRank.begin();
        if (scoreRank.size() > 1)
        {
            Answer::Random* rnd = Answer::Singleton<Answer::Random>::instance();
            int32_t idx = Answer::Random::generate(rnd, 0, (int32_t)scoreRank.size() - 1);
            std::advance(randIt, idx);
        }
        PlayerScore* pWinner = *randIt;
        m_NewWinerId = pWinner->nCharId;
        Player::getName(GameService::getPlayer(Answer::Singleton<GameService>::instance(), pWinner->nCharId, 0, 0), &m_NewWinerName);
        m_Battle = pWinner->nBattle;
        m_ConnId = pWinner->nConnId;
        SetWinnerInfo(m_NewWinerId, &m_NewWinerName);
    }
}

void CrossTower::SetWinnerInfo(CharId_t winner, std::string* p_sName)
{
    m_NewWinerId = winner;
    if (p_sName)
        m_NewWinerName = *p_sName;
}
