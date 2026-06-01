#include "Other/CMapRunner.h"

#include <cstring>

CMapRunner::CMapRunner()
    : Answer::Thread()
{
}

void CMapRunner::run()
{
    CTimer *pTimer = CTimer::instance();
    int64_t nLastTick = CTimer::GetTick(pTimer);
    while (m_bRunning)
    {
        int64_t nCurTick = CTimer::GetTick(pTimer);
        if (nCurTick - nLastTick >= 50)
        {
            nLastTick = nCurTick;
            getLocalNow(m_tmLocalNow);
            update();
            checkMessage();
        }
        Answer::Thread::Sleep(1);
    }
}

void CMapRunner::getLocalNow(tm &retstr)
{
    retstr = m_tmLocalNow;
}

void CMapRunner::Stop()
{
    m_bRunning = false;
    Answer::Thread::join(this);
}

void CMapRunner::AddMap(Map *pMap)
{
    m_maps.push_back(pMap);
}

void CMapRunner::DelMap(MapId_t nMapId)
{
    for (auto it = m_maps.begin(); it != m_maps.end(); ++it)
    {
        if (*it && (*it)->getId() == nMapId)
        {
            m_maps.erase(it);
            break;
        }
    }
}

void CMapRunner::update()
{
    for (auto it = m_maps.begin(); it != m_maps.end(); ++it)
    {
        Map *pMap = *it;
        if (pMap)
            pMap->update();
    }
}

void CMapRunner::checkMessage()
{
    while (true)
    {
        GameMsg *msg = m_msgQueue.pop();
        if (!msg)
            break;
        dealMessage(msg);
        CPoolManager::free(msg);
    }
}

void CMapRunner::dealMessage(GameMsg *msg)
{
    if (!msg)
        return;

    switch (msg->msgcode)
    {
    case GameMsgCode::GMC_PLAYER_ENTER_MAP:
        onPlayerEnterMap((Player *)msg->ptr1, (Map *)msg->ptr2, msg->i32param1, msg->i32param2);
        break;
    case GameMsgCode::GMC_PLAYER_LEAVE_MAP:
        onPlayerLeaveMap((Player *)msg->ptr1, (Map *)msg->ptr2, msg->i32param1, msg->i32param2);
        break;
    case GameMsgCode::GMC_ADD_DUNGEON:
        onAddDungeon((Dungeon *)msg->ptr1);
        break;
    case GameMsgCode::GMC_DEL_DUNGEON:
        onDelDungeon((Dungeon *)msg->ptr1);
        break;
    case GameMsgCode::GMC_PLAYER_LOGOUT:
        onPlayerLogout((Player *)msg->ptr1);
        break;
    case GameMsgCode::GMC_ADD_TERRITORY:
        onAddTerritory((CTerritory *)msg->ptr1);
        break;
    case GameMsgCode::GMC_DEL_TERRITORY:
        onDelTerritory((CTerritory *)msg->ptr1);
        break;
    case GameMsgCode::GMC_ACTIVITY_START:
        onActivityStart((CActivityMap *)msg->ptr1, (CActivity *)msg->ptr2);
        break;
    case GameMsgCode::GMC_ACTIVITY_STOP:
        onActivityStop((CActivityMap *)msg->ptr1, (CActivity *)msg->ptr2);
        break;
    }
}

void CMapRunner::onPlayerEnterMap(Player *pPlayer, Map *pMap, int32_t x, int32_t y)
{
    if (!pPlayer || !pMap)
        return;

    int32_t nRunnerId = pMap->getRunnerId();
    int16_t nGateIndex = Player::getGateIndex(pPlayer);
    CharId_t nCid = Player::getCid(pPlayer);

    MapManager *pMapMgr = MapManager::instance();
    MapManager::onPlayerEnterMap(pMapMgr, pPlayer, pMap, x, y);

    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, Player::getConnId(pPlayer), Answer::PackType::PACK_DISPATCH, 0x1277);
    if (packet)
    {
        int16_t nPlayerGateIndex = Player::getGateIndex(pPlayer);
        packet->writeInt16(nPlayerGateIndex);
        packet->writeInt32(nRunnerId);
        packet->writeInt32(pMap->getId());
        packet->writeInt32(x);
        packet->writeInt32(y);
        uint32_t nWOffset = packet->getWOffset();
        packet->setSize(nWOffset);
        GameService::sendPacket(pGame, Player::getConnId(pPlayer), packet);
    }
}

void CMapRunner::onPlayerLeaveMap(Player *pPlayer, Map *pMap, int32_t x, int32_t y)
{
    if (!pPlayer || !pMap)
        return;

    int16_t nGateIndex = Player::getGateIndex(pPlayer);
    CharId_t nCid = Player::getCid(pPlayer);
    int32_t nRunnerId = pMap->getRunnerId();

    MapManager *pMapMgr = MapManager::instance();
    MapManager::onPlayerLeaveMap(pMapMgr, pPlayer, pMap, x, y);
}

void CMapRunner::onAddDungeon(Dungeon *pDungeon)
{
    if (!pDungeon)
        return;

    m_maps.push_back(pDungeon);
    MapManager *pMapMgr = MapManager::instance();
    MapManager::onAddMap(pMapMgr, pDungeon);
}

void CMapRunner::onDelDungeon(Dungeon *pDungeon)
{
    if (!pDungeon)
        return;

    int32_t nId = pDungeon->getId();
    MapManager *pMapMgr = MapManager::instance();

    for (auto it = m_maps.begin(); it != m_maps.end(); ++it)
    {
        if (*it && (*it)->getId() == nId)
        {
            m_maps.erase(it);
            break;
        }
    }
    MapManager::onDelMap(pMapMgr, pDungeon);
}

void CMapRunner::onPlayerLogout(Player *pPlayer)
{
    if (!pPlayer)
        return;

    int16_t nGateIndex = Player::getGateIndex(pPlayer);
    int32_t nConnId = Player::getConnId(pPlayer);
    CharId_t nCid = Player::getCid(pPlayer);

    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x1314);
    if (packet)
    {
        packet->writeInt16(nGateIndex);
        packet->writeInt64(nCid);
        uint32_t nWOffset = packet->getWOffset();
        packet->setSize(nWOffset);
        GameService::sendPacket(pGame, 0, packet);
    }

    Map *pMap = Player::getMap(pPlayer);
    if (pMap)
        Map::removePlayer(pMap, pPlayer);
}

void CMapRunner::onAddTerritory(CTerritory *pTerritory)
{
    if (!pTerritory)
        return;

    m_maps.push_back(pTerritory);
    MapManager *pMapMgr = MapManager::instance();
    MapManager::onAddMap(pMapMgr, pTerritory);
}

void CMapRunner::onDelTerritory(CTerritory *pTerritory)
{
    if (!pTerritory)
        return;

    FamilyId_t nTerritoryId = pTerritory->getFamilyId();
    MapManager *pMapMgr = MapManager::instance();

    for (auto it = m_maps.begin(); it != m_maps.end(); ++it)
    {
        if (*it && (*it)->getId() == pTerritory->getId())
        {
            m_maps.erase(it);
            break;
        }
    }
    MapManager::onDelMap(pMapMgr, pTerritory);
}

void CMapRunner::onActivityStart(CActivityMap *pMap, CActivity *pActivity)
{
    if (pMap && pActivity)
        pMap->onActivityStart(pActivity);
}

void CMapRunner::onActivityStop(CActivityMap *pMap, CActivity *pActivity)
{
    if (pMap && pActivity)
        pMap->onActivityStop(pActivity);
}

void CMapRunner::onStarCityWar(Map *pMap)
{
    if (pMap)
        pMap->AllPlayerBackCity(pMap);
}

CMapRunner::~CMapRunner()
{
}
