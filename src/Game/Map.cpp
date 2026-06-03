#include "common.h"
#include "Game/Map.h"
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Game/CPoolManager.h"
#include "Game/CTimer.h"
#include "Config/CfgData.h"
#include "Config/CfgMap.h"
#include "Config/CfgMapEvent.h"
#include "Other/Monster.h"
#include "Other/Npc.h"
#include "Other/NpcDungeon.h"
#include "Other/Plant.h"
#include "Other/Trap.h"
#include "Other/Trailer.h"
#include "Other/CDropItem.h"
#include "Other/CDropItemGroup.h"
#include "Other/CLittleHelper.h"
#include "Other/CObjPet.h"
#include "Other/CWorldBoss.h"
#include "Other/TileManager.h"
#include "Other/GroupMonster.h"
#include "Other/Logger.h"
#include "Map/MapRunner.h"
#include "Map/IMapEvent.h"

#include <cstring>
#include <cmath>
#include <algorithm>

// ============================================================
// 构造 / 析构
// ============================================================

Map::Map()
{
    CfgMap::CfgMap(&m_cfgmap);
    m_pMapRunner = 0;
    // STL containers default-constructed
    m_NeedUpdate = 0;
    m_IsStart = 0;
    memset(&m_lastLocalNow, 0, sizeof(m_lastLocalNow));
}

Map::~Map()
{
    // STL containers destroyed automatically
}

// ============================================================
// 坐标转换
// ============================================================

Position Map::pixelToTile(int32_t px, int32_t py)
{
    int32_t tx = px / 48;
    int32_t ty = py / 34;
    return Position(tx, ty);
}

Position Map::tileToPixel(int32_t tx, int32_t ty)
{
    int32_t px = 48 * tx + 24;
    int32_t py = 34 * ty + 17;
    return Position(px, py);
}

// ============================================================
// 方向计算
// ============================================================

Direction Map::pixelDirection(int32_t cx_0, int32_t cy, int32_t tx, int32_t ty)
{
    double degrees = 180.0 * atan2((double)(ty - cy), (double)(tx - cx_0)) / 3.14159265;
    int32_t v4 = (int32_t)round(degrees / 45.0);
    Direction direction = (Direction)(v4 + (v4 < 0 ? 8 : 0));

    switch (direction)
    {
        case Direction::DOWN:       return Direction::RIGHT;
        case Direction::DOWN_RIGHT: return Direction::DOWN_RIGHT;
        case Direction::RIGHT:      return Direction::DOWN;
        case Direction::UP_RIGHT:   return Direction::DOWN_LEFT;
        case Direction::UP:         return Direction::LEFT;
        case Direction::UP_LEFT:    return Direction::UP_LEFT;
        case Direction::LEFT:       return Direction::UP;
        case Direction::DOWN_LEFT:  return Direction::UP_RIGHT;
        default:
            if (direction == Direction::INVALID_DIRECTION)
                LOG_ERROR("Map::pixelDirection return INVALID_DIRECTION with cx=%d,cy=%d,tx=%d,ty=%d", cx_0, cy, tx, ty);
            return direction;
    }
}

Direction Map::tileDirection(int32_t cx_0, int32_t cy, int32_t tx, int32_t ty)
{
    if (cx_0 < 0 || cy < 0 || tx < 0 || ty < 0)
        return Direction::INVALID_DIRECTION;
    if (cx_0 == tx && cy == ty)
        return Direction::INVALID_DIRECTION;
    if (cx_0 == tx && cy > ty) return Direction::UP;
    if (cx_0 == tx && cy < ty) return Direction::DOWN;
    if (cy == ty && cx_0 > tx) return Direction::LEFT;
    if (cy == ty && cx_0 < tx) return Direction::RIGHT;
    if (cy <= ty)
    {
        if (cx_0 > tx && cx_0 - tx == ty - cy) return Direction::DOWN_LEFT;
        if (cx_0 < tx && tx - cx_0 == ty - cy) return Direction::DOWN_RIGHT;
    }
    else
    {
        if (cx_0 > tx && cx_0 - tx == cy - ty) return Direction::UP_LEFT;
        if (cx_0 < tx && tx - cx_0 == cy - ty) return Direction::UP_RIGHT;
    }
    return Direction::INVALID_DIRECTION;
}

Direction Map::UnitDirection(const Position *src, const Position *tar)
{
    Direction direction = tileDirection(src->x, src->y, tar->x, tar->y);
    if (direction == Direction::INVALID_DIRECTION)
    {
        Position srcPixel = tileToPixel(src->x, src->y);
        Position tarPixel = tileToPixel(tar->x, tar->y);
        return pixelDirection(srcPixel.x, srcPixel.y, tarPixel.x, tarPixel.y);
    }
    return direction;
}

Position Map::getAroundTile(int32_t tx, int32_t ty, Direction direction)
{
    switch (direction)
    {
        case Direction::DOWN:       return Position(tx, ty + 1);
        case Direction::DOWN_RIGHT: return Position(tx + 1, ty + 1);
        case Direction::RIGHT:      return Position(tx + 1, ty);
        case Direction::UP_RIGHT:   return Position(tx + 1, ty - 1);
        case Direction::UP:         return Position(tx, ty - 1);
        case Direction::UP_LEFT:    return Position(tx - 1, ty - 1);
        case Direction::LEFT:       return Position(tx - 1, ty);
        case Direction::DOWN_LEFT:  return Position(tx - 1, ty + 1);
        default:                    return Position(tx, ty);
    }
}

Position Map::getAroudFreeTileBuyDir(int32_t tx, int32_t ty, Direction direction)
{
    if (direction == Direction::INVALID_DIRECTION)
        return Position(0, 0);

    Int32List vDir;
    vDir.push_back((int32_t)direction);
    vDir.push_back((direction + 2) % 8);
    vDir.push_back((direction + 6) % 8);
    vDir.push_back((direction + 1) % 8);
    vDir.push_back((direction + 7) % 8);

    for (auto dir : vDir)
    {
        Position pos = getAroundTile(tx, ty, (Direction)dir);
        if (!(pos.x == tx && pos.y == ty)
            && isWalkablePosition(pos.x, pos.y)
            && !HaveUnit(pos.x, pos.y))
        {
            return pos;
        }
    }
    return Position(0, 0);
}

Position Map::getAroudFreeTile(int32_t tx, int32_t ty)
{
    for (int32_t i = 0; i <= 7; ++i)
    {
        Position pos = getAroundTile(tx, ty, (Direction)i);
        if (!(pos.x == tx && pos.y == ty)
            && isWalkablePosition(pos.x, pos.y)
            && !HaveUnit(pos.x, pos.y))
        {
            return pos;
        }
    }
    return Position(0, 0);
}

// ============================================================
// 初始化
// ============================================================

void Map::init(CMapRunner *pRunner, const CfgMap *cfgmap)
{
    m_cfgmap = *cfgmap;
    m_pMapRunner = pRunner;
    m_widthTileCount = m_cfgmap.width / 48;
    m_heightTileCount = (int32_t)round((double)m_cfgmap.height / 17.0);
    m_widthAreaCount = m_cfgmap.width / 232 + 1;
    m_heightAreaCount = m_cfgmap.height / 162 + 1;
    m_lastUpdateTime = 0;
    m_StartandEndTime = 0;
    m_IsStart = 0;

    int32_t MapId = GetMapId();
    CfgData *pCfgData = CfgData::GetInstance();
    CfgMapMonsterVector *pMonsters = pCfgData->getMonstersOnMap(MapId);

    if (pMonsters)
    {
        for (auto &mapMonster : *pMonsters)
        {
            if (mapMonster.hide) continue;
            // Day check
            if (mapMonster.Day > 0 && mapMonster.Day > pCfgData->getServerDiffDay(SERVER_TYPE::SVT_NORMAL) + 1)
                continue;

            const CfgMonster *pCfgMonster = pCfgData->getMonster(mapMonster.monsterid);
            if (!pCfgMonster) continue;

            if (pCfgMonster->boss_sign <= 0)
            {
                CPoolManager *poolMgr = CPoolManager::GetInstance();
                Monster *monster = poolMgr->Pop<Monster>();
                if (monster)
                {
                    AttrAddonList vAttrAddon;
                    Monster::init(monster, pCfgMonster, &mapMonster, MonsterState::MS_STAND, &vAttrAddon);
                    addMonster(monster, mapMonster.x, mapMonster.y);
                }
            }
            else
            {
                CWorldBoss *worldBoss = CWorldBoss::GetInstance();
                worldBoss->OnAddBoss(this, pCfgMonster, &mapMonster);
            }
        }
    }

    // Group monsters
    GroupMonsterVector GMonster;
    pCfgData->GetGroupMonsterVector(&GMonster, MapId);
    for (auto &group : GMonster)
    {
        if (group.IsSpecial <= 0)
        {
            int32_t AgvCount = group.MonsterCount / group.RegionCount;
            if (AgvCount <= 0)
            {
                LOG_ERROR("GroupMonster error mapid=%d,mid=%d", cfgmap->id, group.Mid);
                continue;
            }

            int32_t CurCount = 0;
            for (int32_t i_0 = 0; i_0 < group.MonsterCount; ++i_0)
            {
                if (group.RegionCount - 1 != CurCount && i_0 && !(i_0 % AgvCount))
                    ++CurCount;

                int32_t MonsterId_0 = pCfgData->GetMapMonsterId();
                CfgMapMonster MapMonster_0;
                memset(&MapMonster_0, 0, sizeof(MapMonster_0));
                MapMonster_0.id = MonsterId_0;
                MapMonster_0.mapid = cfgmap->id;

                int32_t v33 = CurCount + 1;
                int32_t id = cfgmap->id;
                TileManager *tileMgr = TileManager::GetInstance();
                Position pos = tileMgr->getRandomWalkablePosition(id, group.RegionCount, v33);
                if (pos.x != -1 && pos.y != -1)
                {
                    MapMonster_0.x = pos.x;
                    MapMonster_0.y = pos.y;

                    const CfgMonster *pMonster_0 = pCfgData->getMonster(group.Mid);
                    CPoolManager *poolMgr = CPoolManager::GetInstance();
                    Monster *monster_1 = poolMgr->Pop<Monster>();
                    if (monster_1 && pMonster_0)
                    {
                        AttrAddonList v76;
                        Monster::init(monster_1, pMonster_0, &MapMonster_0, MonsterState::MS_STAND, &v76);
                        Monster::SetGroupMonster(monster_1, group.RegionCount, CurCount + 1);
                        addMonster(monster_1, MapMonster_0.x, MapMonster_0.y);
                    }
                }
            }
            m_MonsterReviveMap[group.Mid] = 0;
        }
        else
        {
            std::vector<Position> RevivePos = group.RevivePos;
            std::random_shuffle(RevivePos.begin(), RevivePos.end());

            for (int32_t i = 0; i < group.MonsterCount && i < (int32_t)group.RevivePos.size(); ++i)
            {
                int32_t MonsterId = pCfgData->GetMapMonsterId();
                CfgMapMonster MapMonster;
                memset(&MapMonster, 0, sizeof(MapMonster));
                MapMonster.id = MonsterId;
                MapMonster.mapid = cfgmap->id;
                MapMonster.x = RevivePos[i].x;
                MapMonster.y = RevivePos[i].y;

                const CfgMonster *pMonster = pCfgData->getMonster(group.Mid);
                CPoolManager *poolMgr = CPoolManager::GetInstance();
                Monster *monster_0 = poolMgr->Pop<Monster>();
                if (monster_0 && pMonster)
                {
                    AttrAddonList v74;
                    Monster::init(monster_0, pMonster, &MapMonster, MonsterState::MS_STAND, &v74);
                    Monster::SetGroupMonster(monster_0, 0, 0);
                    Monster::SetRevivePos(monster_0, &RevivePos);
                    addMonster(monster_0, MapMonster.x, MapMonster.y);
                }
            }
            m_MonsterReviveMap[group.Mid] = 0;
        }
    }

    // NPCs
    const CfgNpcTable *allNpc = pCfgData->getNpcAll();
    if (allNpc)
    {
        for (auto &pair : *allNpc)
        {
            const CfgNpc &cfgNpc = pair.second;
            if (CfgNpc::findMapId(&cfgNpc, MapId) && cfgNpc.x > 0 && cfgNpc.y > 0)
            {
                if (cfgNpc.unite_flag != 1 || pCfgData->isUniteServer())
                {
                    Npc *npc = new Npc();
                    if (npc)
                    {
                        Npc::init(npc, &cfgNpc, 0);
                        addNpc(npc, cfgNpc.x, cfgNpc.y);
                    }
                }
            }
        }
    }

    // Plants
    CfgMapPlantVector *pPlants = pCfgData->getPlantOnMap(MapId);
    if (pPlants)
    {
        for (auto &mapPlant : *pPlants)
        {
            const CfgPlant *pCfgPlant = pCfgData->getPlant(mapPlant.plantid);
            if (pCfgPlant
                && (!pCfgPlant->type == 0
                    || pCfgPlant->type == 3
                    || pCfgPlant->type == 4
                    || pCfgPlant->type == 6
                    || pCfgPlant->type == 7
                    || pCfgPlant->type == 8))
            {
                Plant *plant = new Plant();
                if (plant)
                {
                    Plant::init(plant, this, pCfgPlant, &mapPlant, 0);
                    addPlant(plant);
                }
            }
        }
    }

    if (IsMoLingHuanYu())
    {
        m_StartandEndTime = Answer::DayTime::now();
        m_IsStart = 1;
    }
}

// ============================================================
// 主循环
// ============================================================

void Map::update()
{
    tm LocalNow;
    getLocalNow(&LocalNow);

    if (!m_players.empty() || !m_trailers.empty()
        || LocalNow.tm_min != m_lastLocalNow.tm_min
        || m_NeedUpdate)
    {
        m_NeedUpdate = 0;
        m_lastUpdateTime = getNow();
        m_lastLocalNow = LocalNow;

        updateGroupMonsterRevive(m_lastUpdateTime);
        updatePlayers();
        updateMonsters();
        updateNpcs();
        updatePlants();
        updateDropItems();
        updateTraps();
        updateTrailers();

        if (IsMoLingHuanYu())
        {
            int32_t MapId = GetMapId();
            CfgData *pCfgData = CfgData::GetInstance();
            SpecialBossMapCfg *pCfg = pCfgData->GetSpecialBossMapCfg(MapId);
            if (pCfg && pCfg->StartCD > 0)
            {
                if (m_IsStart)
                {
                    if (m_lastUpdateTime - m_StartandEndTime > 60 * pCfg->ContinuedTime)
                    {
                        m_StartandEndTime = m_lastUpdateTime;
                        m_IsStart = 0;
                    }
                }
                else if (m_lastUpdateTime - m_StartandEndTime > 60 * pCfg->StartCD)
                {
                    m_StartandEndTime = m_lastUpdateTime;
                    m_IsStart = 1;
                    GameService *gs = GameService::GetInstance();
                    Answer::NetPacket *packet = gs->popNetpacket(0, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                    if (packet)
                    {
                        packet->writeInt32(476);
                        packet->setSize(packet->getWOffset());
                        gs->worldBroadcast(0, packet);
                    }
                }
            }
        }
    }
}

bool Map::IsCrossMap()
{
    return CfgMap::isCross(&m_cfgmap, CROSS_MAP_TYPE::CMT_CROSS);
}

void Map::ResetRefreshMonster()
{
    int32_t nNow = getNow();
    for (auto &pair : m_MonsterReviveMap)
    {
        if (pair.second > 0)
            pair.second = nNow;
    }
    m_NeedUpdate = 1;
}

void Map::updateGroupMonsterRevive(int32_t Now)
{
    for (auto &pair : m_MonsterReviveMap)
    {
        if (pair.second > 0 && pair.second <= Now)
        {
            int32_t ReviveTime = 0;
            for (auto *monster : m_monsters)
            {
                if (monster)
                {
                    int32_t Mid = Monster::getMid(monster);
                    if (Mid == pair.first && Monster::IsGroupMonster(monster))
                    {
                        Monster::SetReviveTime(monster, pair.second);
                        const CfgMonster *pMonster = CfgData::GetInstance()->getMonster(pair.first);
                        if (pMonster)
                        {
                            ReviveTime = CfgData::GetInstance()->GetMonsterReviveTime(
                                pMonster->revive_time, pMonster->boss_sign) / 1000;
                        }
                    }
                }
            }
            if (ReviveTime > 0)
                pair.second += ReviveTime;
        }
    }
}

// ============================================================
// 进入检查
// ============================================================

int32_t Map::canEnter(Player *player)
{
    if (!player) return 10002;

    int32_t lv = Player::GetLevel(player);
    if (lv < m_cfgmap.player_level || lv > m_cfgmap.player_level_max)
        return 10056;

    if (m_cfgmap.XingMaiLevel > 0 && Player::getRecord(player, 1147) < m_cfgmap.XingMaiLevel)
        return 10002;

    GameService *gs = GameService::GetInstance();
    if (gs->getLine() != 9 && m_cfgmap.LimitDay > 0)
    {
        CfgData *pCfgData = CfgData::GetInstance();
        if (pCfgData->getServerDiffDay(SERVER_TYPE::SVT_NORMAL) + 1 < m_cfgmap.LimitDay)
            return 10002;
    }

    if (m_cfgmap.SuitId > 0)
    {
        CExtEquip *Equip = Player::GetEquip(player);
        if (m_cfgmap.SuitId > Equip->GetCount12SuitId())
            return 10002;
    }
    if (m_cfgmap.VipLevel > 0)
    {
        CVip *PlayerVip = Player::GetVip(player);
        if (m_cfgmap.VipLevel > PlayerVip->GetVipLevel())
            return 10002;
    }
    if (m_cfgmap.JueWeiLevel > 0)
    {
        CExtCharJueWei *CharJueWei = Player::GetJueWei(player);
        if (m_cfgmap.JueWeiLevel > CharJueWei->GetLevel())
            return 10002;
    }

    if (m_cfgmap.FamilyType > 0)
    {
        CActivityManager *actMgr = CActivityManager::GetInstance();
        if (!actMgr->IsInFamilyActivity())
            return 10002;
    }
    if (m_cfgmap.CrossType == 1)
    {
        CActivityManager *actMgr = CActivityManager::GetInstance();
        if (!actMgr->IsActBegin(8))
            return 10002;
    }
    if (m_cfgmap.mType == 1 && m_cfgmap.mType2 == 1)
    {
        if (Player::GetFamilyId(player) <= 0)
            return 10002;
        FamilyId_t FamilyId = Player::GetFamilyId(player);
        int8_t v16 = Player::GetConnId(player);
        CActivityManager *actMgr = CActivityManager::GetInstance();
        if (!actMgr->IsInFamilyWar(v16, FamilyId))
            return 10002;
    }
    if (m_cfgmap.mType == 1 && m_cfgmap.mType2 == 3)
    {
        CActivityManager *actMgr = CActivityManager::GetInstance();
        if (!actMgr->IsInArena())
            return 10002;
    }
    // Check open time
    {
        tm time;
        getLocalNow(&time);
        if (!CfgMap::isOpenTime(&m_cfgmap, &time))
            return 10002;
    }

    if (m_cfgmap.mType == 1 && m_cfgmap.mType2 == 5)
    {
        CXinMo *xinMo = CXinMo::GetInstance();
        if (!xinMo->isOpen())
            return 10002;
    }
    if (m_cfgmap.mType == 1 && m_cfgmap.mType2 == 6)
    {
        CXinMo *xinMo = CXinMo::GetInstance();
        if (!xinMo->isOpen())
            return 10002;
    }

    return 0;
}

bool Map::SpecialSitRevive()
{
    return IsXinMoMap();
}

bool Map::CanAutoRevive()
{
    int32_t MapId = GetMapId();
    if (MapId == 50167) return true;
    if (MapId > 50167)
    {
        if (MapId >= 50169 && (MapId <= 50171 || MapId == 51100))
            return true;
    }
    else
    {
        if (MapId >= 1057 && MapId <= 1061) return true;
        if (MapId == 2004) return true;
        if (MapId == 3007) return true;
        if (MapId >= 40001 && MapId <= 40009) return true;
        if (MapId == 50008) return true;
        if (MapId >= 50010 && MapId <= 50015) return true;
    }
    return false;
}

void Map::onMonsterDie(Monster *monster, Player *player, bool IsGroupMonster)
{
    if (!monster) return;

    int32_t MapId = GetMapId();
    CfgData *pCfgData = CfgData::GetInstance();
    int32_t Mid = Monster::getMid(monster);

    if (IsGroupMonster)
    {
        auto it = m_MonsterReviveMap.find(Mid);
        if (it != m_MonsterReviveMap.end())
        {
            int32_t boss_sign = 0;
            int32_t revive_time = 0;
            const CfgMonster *pMonster = pCfgData->getMonster(Mid);
            if (pMonster)
            {
                boss_sign = pMonster->boss_sign;
                revive_time = pMonster->revive_time;
            }
            int32_t ReviveTime = pCfgData->GetMonsterReviveTime(revive_time, boss_sign) / 1000;
            if (ReviveTime > 0)
                it->second = getNow() + ReviveTime;
            else
                it->second = -1;
        }
    }
}

bool Map::OnSitRevive(Player *player)
{
    if (!player) return false;

    if (IsXinMoMap()
        && Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, 500,
            CURRENCY_CHANGE_REASON::GCR_XINMO_SIT_REVIVE, 0))
    {
        int32_t Now = getNow();
        Player::SetStartProtect(player, Now);
        Player::FullHPAndSync(player);
        Unit::instantMove(player, m_cfgmap.x, m_cfgmap.y, InstanceMoveReason::IMR_TRANSFER, 0);
        return true;
    }
    return false;
}

bool Map::CanUsePet()
{
    return m_cfgmap.MapCanUsePet > 0;
}

bool Map::CanStayInMap()
{
    return m_cfgmap.SignOut <= 0;
}

int32_t Map::GetAvgBattle()
{
    if (m_players.empty())
        return 0;

    int64_t Battle = 0;
    int32_t nSize = (int32_t)m_players.size();
    for (auto *player : m_players)
    {
        if (player)
            Battle += Player::GetBattle(player);
    }
    return (int32_t)(Battle / nSize);
}

int64_t Map::getTick()
{
    if (m_pMapRunner)
        return m_pMapRunner->getTick();
    CTimer *timer = CTimer::GetInstance();
    return timer->GetTick();
}

int32_t Map::getNow()
{
    if (m_pMapRunner)
        return m_pMapRunner->getNow();
    CTimer *timer = CTimer::GetInstance();
    return timer->GetNow();
}

void Map::getLocalNow(tm *retstr)
{
    if (m_pMapRunner)
        m_pMapRunner->getLocalNow(retstr);
    else
        CTimer::GetInstance()->GetLocalNow(retstr);
}

Tile *Map::getTile(int32_t tx, int32_t ty)
{
    if (tx >= 0 && m_widthTileCount > tx && ty >= 0 && m_heightTileCount > ty)
    {
        TileManager *tileMgr = TileManager::GetInstance();
        return tileMgr->getTile(m_cfgmap.id, tx, ty);
    }
    return nullptr;
}

bool Map::isWalkablePosition(int32_t tx, int32_t ty)
{
    if (inStack(tx, ty))
        return false;
    if (tx < 0 || m_widthTileCount <= tx || ty < 0 || m_heightTileCount <= ty)
        return false;

    Tile *pTile = getTile(tx, ty);
    if (!pTile)
        return false;

    int32_t MapId = GetMapId();
    TileManager *tileMgr = TileManager::GetInstance();
    return tileMgr->isWalkAble(tileMgr, MapId, tx, ty);
}

bool Map::inStack(int32_t tx, int32_t ty) const
{
    if (m_lstStack.empty())
        return false;

    for (auto &pos : m_lstStack)
    {
        if (pos.x == tx && pos.y == ty)
            return true;
    }
    return false;
}

Position Map::getAreaByPos(const Position *pos)
{
    return Position(pos->x / 232, pos->y / 162);
}

int32_t Map::getViewAreaCount(int32_t nSize) const
{
    return (2 * nSize + 1) * (2 * nSize + 1);
}

// ============================================================
// 区域广播
// ============================================================

void Map::broadcastToAreas(Answer::NetPacket *packet, const PositionVector *areas)
{
    if (!packet || !areas) return;

    IndexMap indexMap;
    for (auto *player : m_players)
    {
        if (!player) continue;
        int16_t GateIndex = Player::GetGateIndex(player);
        int8_t ConnId = Player::GetConnId(player);
        int16_t key = (int16_t)ConnId;
        auto it = indexMap.find(key);
        if (it == indexMap.end())
        {
            Int8List list;
            list.push_back(ConnId);
            indexMap[key] = list;
        }
        else
        {
            it->second.push_back(ConnId);
        }
    }

    GameService *gs = GameService::GetInstance();
    for (auto &entry : indexMap)
    {
        int16_t GateIndex = entry.first;
        auto &list = entry.second;
        gs->sendPacketToArea(list, GateIndex, packet);
    }
}

// ============================================================
// 玩家管理
// ============================================================

void Map::addPlayer(Player *player, int32_t x, int32_t y)
{
    if (!player) return;

    Unit::setAtTile(player, this, x, y);
    m_players.push_back(player);

    Player::GetBag(player);  // Ensure bag initialized

    // Pet
    CObjPet *pet = Player::GetPet(player);
    if (pet)
        addPet(pet);

    // Little helper
    CLittleHelper *helper = Player::GetLittleHelper(player);
    if (helper)
        AddLittleHelper(helper);

    sendEnterMap(player, x, y);

    int32_t RunnerId = GetRunnerId();
    GameService *gs = GameService::GetInstance();
    gs->PlayerEnterMap(player, RunnerId);
}

void Map::removePlayer(Player *player, bool islogout)
{
    if (!player) return;

    Unit::setAtTile(player, 0, 0, 0);
    m_players.remove(player);

    // Remove pets
    CObjPet *pet = Player::GetPet(player);
    if (pet)
        removePet(pet);

    // Remove little helper
    CLittleHelper *helper = Player::GetLittleHelper(player);
    if (helper)
        removeLittleHelper(helper);

    broadcastLeaveMap(player);
}

void Map::GetNearestAliveEvilUnit(Monster *monster)
{
    if (!monster) return;

    CExtFightChecker *FightChecker = Monster::GetFightChecker(monster);
    Position CurrentTile = Unit::getCurrentTile(monster);
    Unit *pUnit = nullptr;
    int32_t mdis = 99999999;

    for (auto *pPlayer : m_players)
    {
        if (!pPlayer || !Unit::isAlive(pPlayer)) continue;
        if (!FightChecker->isEvil(pPlayer)) continue;

        Position pos = Unit::getCurrentTile(pPlayer);
        int32_t dx = pos.x - CurrentTile.x;
        int32_t dy = pos.y - CurrentTile.y;
        int32_t dis = dx * dx + dy * dy;
        if (dis < mdis)
        {
            mdis = dis;
            pUnit = pPlayer;
        }
    }
    Monster::SetAttackTarget(monster, pUnit);
}

void Map::GetNearestAliveEnemySideUnit(Monster *monster)
{
    if (!monster) return;

    int8_t Camp = Monster::GetCamp(monster);
    FamilyId_t FamilyId = Monster::GetFamilyId(monster);
    int32_t Side = Monster::GetSide(monster);
    Position CurrentTile = Unit::getCurrentTile(monster);
    Unit *pUnit = nullptr;
    int32_t mdis = 99999999;

    for (auto *pPlayer : m_players)
    {
        if (!pPlayer || !Unit::isAlive(pPlayer)) continue;
        int8_t pCamp = Player::GetCamp(pPlayer);
        FamilyId_t pFamily = Player::GetFamilyId(pPlayer);
        int32_t pSide = Player::GetSide(pPlayer);

        bool isEnemy = false;
        if (Side > 0 && pSide > 0 && Side != pSide)
            isEnemy = true;
        else if (FamilyId > 0 && pFamily > 0 && FamilyId != pFamily)
            isEnemy = true;
        else if (Camp != pCamp)
            isEnemy = true;

        if (!isEnemy) continue;

        Position pos = Unit::getCurrentTile(pPlayer);
        int32_t dx = pos.x - CurrentTile.x;
        int32_t dy = pos.y - CurrentTile.y;
        int32_t dis = dx * dx + dy * dy;
        if (dis < mdis)
        {
            mdis = dis;
            pUnit = pPlayer;
        }
    }
    Monster::SetAttackTarget(monster, pUnit);
}

void Map::GetNearestDropItem(CLittleHelper *pLittleHelper)
{
    if (!pLittleHelper) return;

    int32_t TeamId = CLittleHelper::GetTeamId(pLittleHelper);
    CharId_t Cid = CLittleHelper::GetCid(pLittleHelper);
    int32_t ItemId = CLittleHelper::GetItemId(pLittleHelper);
    int8_t ItemClass = CLittleHelper::GetItemClass(pLittleHelper);
    Position pos = Unit::getCurrentTile(pLittleHelper);
    CDropItem *pTarget = nullptr;
    int32_t mdis = 99999999;

    for (auto *pDropItem : m_dropItems)
    {
        if (!pDropItem || CDropItem::IsLocked(pDropItem)) continue;
        if (CDropItem::GetTeamId(pDropItem) != TeamId) continue;
        if (CDropItem::GetOwner(pDropItem) != Cid) continue;

        if (ItemId > 0)
        {
            if (CDropItem::GetItemId(pDropItem) != ItemId) continue;
        }
        if (ItemClass > 0)
        {
            if (CDropItem::GetItemClass(pDropItem) != ItemClass) continue;
        }

        Position dpos = CDropItem::GetPosition(pDropItem);
        int32_t dx = dpos.x - pos.x;
        int32_t dy = dpos.y - pos.y;
        int32_t dis = dx * dx + dy * dy;
        if (dis < mdis)
        {
            mdis = dis;
            pTarget = pDropItem;
        }
    }
    CLittleHelper::SetTarget(pLittleHelper, pTarget);
}

void Map::GetNearestAliveEnemySideUnit(CObjPet *Pet)
{
    if (!Pet) return;

    Position CurrentTile = CObjPet::GetCurrentTile(Pet);
    Unit *pUnit = nullptr;
    int32_t mdis = 99999999;

    for (auto *pMonster : m_monsters)
    {
        if (!pMonster || !Unit::isAlive(pMonster)) continue;

        Position mpos = Unit::getCurrentTile(pMonster);
        int32_t dx = mpos.x - CurrentTile.x;
        int32_t dy = mpos.y - CurrentTile.y;
        int32_t dis = dx * dx + dy * dy;
        if (dis < mdis)
        {
            mdis = dis;
            pUnit = pMonster;
        }
    }
    CObjPet::SetAttackTarget(Pet, pUnit);
}

// ============================================================
// 实体管理
// ============================================================

void Map::addMonster(Monster *monster, int32_t x, int32_t y)
{
    if (!monster) return;
    Unit::setAtTile(monster, this, x, y);
    m_monsters.push_back(monster);
    broadcastIntoMap(monster);
}

void Map::addNpc(Npc *npc, int32_t x, int32_t y)
{
    if (!npc) return;
    Unit::setAtTile(npc, this, x, y);
    m_npcs.push_back(npc);
    broadcastIntoMap(npc);
}

Npc *Map::getNpc(EntityId_t id)
{
    for (auto *npc : m_npcs)
    {
        if (npc && Npc::GetId(npc) == id)
            return npc;
    }
    return nullptr;
}

void Map::addDropItem(CDropItem *dropItem)
{
    if (!dropItem) return;
    m_dropItems.push_back(dropItem);
}

bool Map::HaveDropAndPlayer(Position Pos)
{
    for (auto *pPlayer : m_players)
    {
        if (pPlayer && Unit::getTileX(pPlayer) == Pos.x && Unit::getTileY(pPlayer) == Pos.y)
            return true;
    }
    for (auto *pDropItem : m_dropItems)
    {
        if (pDropItem && CDropItem::GetX(pDropItem) == Pos.x && CDropItem::GetY(pDropItem) == Pos.y)
            return true;
    }
    return false;
}

void Map::addDropItemGroup(const CDropItemGroup *dropItemGroup)
{
    if (!dropItemGroup) return;
    const DropItemList *dropItemList = dropItemGroup->GetDropItemList();
    if (!dropItemList) return;
    for (auto *pItem : *dropItemList)
    {
        m_dropItems.push_back(pItem);
    }
}

CDropItem *Map::getDropItem(EntityId_t dropid)
{
    for (auto *pItem : m_dropItems)
    {
        if (pItem && CDropItem::GetId(pItem) == dropid)
            return pItem;
    }
    return nullptr;
}

void Map::addPlant(Plant *plant)
{
    if (!plant) return;
    m_plants.push_back(plant);
}

Plant *Map::getPlant(EntityId_t id)
{
    for (auto *plant : m_plants)
    {
        if (plant && Plant::GetId(plant) == id)
            return plant;
    }
    return nullptr;
}

void Map::removePlant(EntityId_t id)
{
    for (auto it = m_plants.begin(); it != m_plants.end(); ++it)
    {
        if (*it && Plant::GetId(*it) == id)
        {
            m_plants.erase(it);
            return;
        }
    }
}

void Map::AddLittleHelper(CLittleHelper *pLittleHelper)
{
    if (!pLittleHelper) return;
    m_CLittleHelperList.push_back(pLittleHelper);
}

void Map::removeLittleHelper(CLittleHelper *pLittleHelper)
{
    if (!pLittleHelper) return;
    m_CLittleHelperList.remove(pLittleHelper);
}

void Map::addTrailer(Trailer *trailer, int32_t x, int32_t y)
{
    if (!trailer) return;
    Unit::setAtTile(trailer, this, x, y);
    m_trailers.push_back(trailer);
    broadcastIntoMap(trailer);
}

void Map::removeTrailer(Trailer *trailer)
{
    if (!trailer) return;
    m_trailers.remove(trailer);
    broadcastLeaveMap(trailer);
}

void Map::addPet(CObjPet *pet)
{
    if (!pet) return;
    m_pets.push_back(pet);
}

void Map::removePet(CObjPet *pet)
{
    if (!pet) return;
    m_pets.remove(pet);
}

void Map::addTrap(Trap *trap)
{
    if (!trap) return;
    m_traps.push_back(trap);
}

void Map::removeTrap(Trap *trap)
{
    if (!trap) return;
    m_traps.remove(trap);
}

Trap *Map::getTrap(EntityId_t id)
{
    for (auto *trap : m_traps)
    {
        if (trap && Trap::GetId(trap) == id)
            return trap;
    }
    return nullptr;
}

// ============================================================
// 区域检测
// ============================================================

bool Map::inSafeRegion(const Position *pos)
{
    int32_t MapId = GetMapId();
    CfgData *pCfgData = CfgData::GetInstance();
    CfgMapRegion *pRegion = pCfgData->getSafeRegion(MapId);
    if (!pRegion) return false;

    Position CurPos = tileToPixel(pos->x, pos->y);
    return CurPos.x >= pRegion->min_x && CurPos.x <= pRegion->max_x
        && CurPos.y >= pRegion->min_y && CurPos.y <= pRegion->max_y;
}

bool Map::inRegion(const Position *pos, const CfgMapRegion *region)
{
    if (!region) return false;
    if (GetMapId() != region->mapid) return false;
    return pos->x >= region->min_x && pos->x <= region->max_x
        && pos->y >= region->min_y && pos->y <= region->max_y;
}

Position Map::getRandomWalkablePosition()
{
    int32_t MapId = GetMapId();
    TileManager *tileMgr = TileManager::GetInstance();
    return tileMgr->getRandomWalkablePosition(MapId);
}

Position Map::getRandomWalkablePositionInRegion(const CfgMapRegion *region)
{
    if (!region) return Position(0, 0);

    Answer::Random *rng = Answer::Random::GetInstance();
    for (int32_t i = 0; i < 20; ++i)
    {
        int32_t x = rng->generate(region->min_x, region->max_x);
        int32_t y = rng->generate(region->min_y, region->max_y);
        if (isWalkablePosition(x, y))
            return Position(x, y);
    }
    return Position(0, 0);
}

Position Map::getRandomWalkableAroundPosition(int32_t tx, int32_t ty)
{
    Position poses[25];
    int32_t count = 0;
    for (int32_t dy = -2; dy <= 2; ++dy)
    {
        for (int32_t dx = -2; dx <= 2; ++dx)
        {
            int32_t nx = tx + dx;
            int32_t ny = ty + dy;
            if (isWalkablePosition(nx, ny) && !HaveUnit(nx, ny))
                poses[count++] = Position(nx, ny);
        }
    }
    if (count <= 0) return Position(0, 0);

    Answer::Random *rng = Answer::Random::GetInstance();
    int32_t idx = rng->generate(0, count - 1);
    return poses[idx];
}

// ============================================================
// 网络同步
// ============================================================

void Map::sendEnterMap(Player *player, int32_t x, int32_t y)
{
    if (!player) return;

    int8_t ConnId = Player::GetConnId(player);
    GameService *gs = GameService::GetInstance();
    Answer::NetPacket *packet = gs->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2710);
    if (!packet) return;

    CharId_t Cid = Player::GetCid(player);
    int32_t RunnerId = GetRunnerId();
    packet->writeInt32(m_cfgmap.id);
    packet->writeInt32(x);
    packet->writeInt32(y);
    packet->writeInt64(Cid);
    packet->writeInt32(RunnerId);
    packet->setSize(packet->getWOffset());

    int16_t GateIndex = Player::GetGateIndex(player);
    gs->sendPacketTo(ConnId, GateIndex, packet);

    sendNpcsInMap(player);
    sendTrailersInMap(player);
    SendStacksInMap(player);
    SendObjsInMap(player);
}

void Map::sendNpcsInMap(Player *player)
{
    if (!player) return;

    int8_t ConnId = Player::GetConnId(player);
    int16_t GateIndex = Player::GetGateIndex(player);
    GameService *gs = GameService::GetInstance();

    for (auto *npc : m_npcs)
    {
        if (!npc) continue;
        Answer::NetPacket *packet = gs->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x271E);
        if (!packet) continue;
        Npc::appendInfo(npc, packet);
        packet->setSize(packet->getWOffset());
        gs->sendPacketTo(ConnId, GateIndex, packet);
    }
}

void Map::sendTrailersInMap(Player *player)
{
    if (!player) return;

    int8_t ConnId = Player::GetConnId(player);
    int16_t GateIndex = Player::GetGateIndex(player);
    GameService *gs = GameService::GetInstance();

    ObjList outObjs;
    for (auto *trailer : m_trailers)
    {
        if (!trailer) continue;
        outObjs.push_back(trailer);
    }

    Answer::NetPacket *packet = gs->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2720);
    if (!packet) return;
    packet->writeInt32((int32_t)outObjs.size());
    for (auto *pObj : outObjs)
    {
        StaticObj::appendInfo(pObj, packet);
    }
    packet->setSize(packet->getWOffset());
    gs->sendPacketTo(ConnId, GateIndex, packet);
}

void Map::sendTeamsInMap(Player *player)
{
    if (!player) return;

    int8_t ConnId = Player::GetConnId(player);
    GameService *gs = GameService::GetInstance();
    Answer::NetPacket *packet = gs->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2721);
    if (!packet) return;

    int32_t count = 0;
    uint32_t savePos = packet->getWOffset();
    packet->writeInt32(0); // placeholder for count

    for (auto *pPlayer : m_players)
    {
        if (!pPlayer || pPlayer == player) continue;
        int32_t TeamId = Player::GetTeamId(pPlayer);
        if (TeamId <= 0) continue;

        packet->writeInt32(TeamId);
        Player::appendSimpleInfo(pPlayer, packet);
        ++count;
    }

    packet->writeInt32At(savePos, count);
    packet->setSize(packet->getWOffset());
    int16_t GateIndex = Player::GetGateIndex(player);
    gs->sendPacketTo(ConnId, GateIndex, packet);
}

void Map::broadcastIntoMap(StaticObj *pObj)
{
    if (!pObj) return;

    GameService *gs = GameService::GetInstance();
    Answer::NetPacket *packet = gs->popNetpacket(0, Answer::PackType::PACK_DISPATCH, 0x271E);
    if (!packet) return;
    StaticObj::appendInfo(pObj, packet);
    packet->setSize(packet->getWOffset());
    broadcast(packet);
}

void Map::broadcastLeaveMap(StaticObj *pObj)
{
    if (!pObj) return;

    GameService *gs = GameService::GetInstance();
    Answer::NetPacket *packet = gs->popNetpacket(0, Answer::PackType::PACK_DISPATCH, 0x271F);
    if (!packet) return;
    packet->writeInt64(StaticObj::GetId(pObj));
    packet->writeInt8(StaticObj::GetObjType(pObj));
    packet->setSize(packet->getWOffset());
    broadcast(packet);
}

void Map::broadcast(Answer::NetPacket *inPacket)
{
    if (!inPacket) return;

    IndexMap indexMap;
    for (auto *player : m_players)
    {
        if (!player) continue;
        int16_t GateIndex = Player::GetGateIndex(player);
        int8_t ConnId = Player::GetConnId(player);
        int16_t key = (int16_t)ConnId;
        auto it = indexMap.find(key);
        if (it == indexMap.end())
        {
            Int8List list;
            list.push_back(ConnId);
            indexMap[key] = list;
        }
        else
        {
            it->second.push_back(ConnId);
        }
    }

    GameService *gs = GameService::GetInstance();
    for (auto &entry : indexMap)
    {
        int16_t GateIndex = entry.first;
        auto &list = entry.second;
        gs->sendPacketTo(list, GateIndex, inPacket);
    }
}

void Map::broadcastAreaAround(Answer::NetPacket *inPacket, StaticObj *pObj)
{
    if (!inPacket || !pObj) return;

    Position playerPos = StaticObj::getCurrentPixel(pObj);
    Position area = getAreaByPos(&playerPos);

    PositionVector areas;
    for (int32_t dy = -1; dy <= 1; ++dy)
    {
        for (int32_t dx = -1; dx <= 1; ++dx)
        {
            areas.push_back(Position(area.x + dx, area.y + dy));
        }
    }

    broadcastToAreas(inPacket, &areas);
}

// ============================================================
// 位置计算
// ============================================================

int32_t Map::xAddHalfY(int32_t x, int32_t y)
{
    return x + y / 2;
}

int32_t Map::xSubHalfY(int32_t x, int32_t y)
{
    return x - y / 2;
}

Position Map::firstLinePosition(Position src, Position dest)
{
    Direction dir = tileDirection(src.x, src.y, dest.x, dest.y);
    if (dir != Direction::INVALID_DIRECTION)
        return getAroundTile(src.x, src.y, dir);
    return src;
}

bool Map::inAreas(StaticObj *pObj, const PositionVector *areas)
{
    if (!pObj || StaticObj::getMap(pObj) != this || !areas)
        return false;

    Position pos = StaticObj::getCurrentPixel(pObj);
    for (auto &area : *areas)
    {
        if (checkPos(&pos, &area))
            return true;
    }
    return false;
}

bool Map::checkPos(const Position *pos, const Position *area) const
{
    int nLeft = 232 * area->x;
    int nBottom = 162 * area->y;
    return pos->x >= nLeft && pos->x < nLeft + 232
        && pos->y >= nBottom && pos->y < nBottom + 162;
}

// ============================================================
// 更新循环
// ============================================================

void Map::updatePlayers()
{
    PlayerList players = m_players; // copy
    for (auto *player : players)
    {
        if (!player) continue;
        if (Player::GetGateIndex(player) < 0)
        {
            removePlayer(player, false);
            continue;
        }
        Unit::update(player);
    }
}

bool Map::IsSpecialBossMap()
{
    return m_cfgmap.type == 1
        && (m_cfgmap.param == 13 || m_cfgmap.param == 15
            || m_cfgmap.param == 16 || m_cfgmap.param == 19
            || m_cfgmap.param == 22);
}

bool Map::IsBeastShrine()
{
    return m_cfgmap.type == 1 && m_cfgmap.param == 22;
}

bool Map::IsMoLingHuanYu()
{
    return m_cfgmap.type == 1 && m_cfgmap.param == 15;
}

bool Map::IsCycleTower()
{
    return m_cfgmap.type == 1 && m_cfgmap.param == 16;
}

bool Map::IsKuangDong()
{
    return m_cfgmap.type == 1 && m_cfgmap.param == 18;
}

bool Map::IsWastelands()
{
    return m_cfgmap.type == 1 && m_cfgmap.param == 19;
}

bool Map::IsXinMoCun()
{
    return m_cfgmap.type == 1 && m_cfgmap.param == 24;
}

bool Map::IsXinMoMap()
{
    return m_cfgmap.type == 1 && m_cfgmap.param == 25;
}

Position Map::GetEnterPos()
{
    return Position(m_cfgmap.x, m_cfgmap.y);
}

void Map::updateMonsters()
{
    auto it = m_monsters.begin();
    while (it != m_monsters.end())
    {
        Monster *monster = *it;
        if (!monster || Unit::IsDestroyed(monster))
        {
            it = m_monsters.erase(it);
            CPoolManager::GetInstance()->Push(monster);
            continue;
        }
        Unit::update(monster);
        ++it;
    }
}

void Map::updateNpcs()
{
    auto it = m_npcs.begin();
    while (it != m_npcs.end())
    {
        Npc *npc = *it;
        if (!npc || Npc::IsDestroyed(npc))
        {
            it = m_npcs.erase(it);
            NpcActivity *npcAct = dynamic_cast<NpcActivity *>(npc);
            if (npcAct)
                CPoolManager::GetInstance()->Push(npcAct);
            else
                delete npc;
            continue;
        }
        ++it;
    }
}

void Map::updatePlants()
{
    auto it = m_plants.begin();
    while (it != m_plants.end())
    {
        Plant *plant = *it;
        if (!plant || Plant::IsDestroyed(plant))
        {
            it = m_plants.erase(it);
            PlantActivity *plantAct = dynamic_cast<PlantActivity *>(plant);
            if (plantAct)
                CPoolManager::GetInstance()->Push(plantAct);
            else
            {
                PlantDungeon *plantDun = dynamic_cast<PlantDungeon *>(plant);
                if (plantDun)
                    CPoolManager::GetInstance()->Push(plantDun);
                else
                    delete plant;
            }
            continue;
        }
        ++it;
    }
}

void Map::updateDropItems()
{
    auto it = m_dropItems.begin();
    while (it != m_dropItems.end())
    {
        CDropItem *pDropItem = *it;
        if (!pDropItem || CDropItem::IsExpired(pDropItem))
        {
            it = m_dropItems.erase(it);
            if (pDropItem)
                CPoolManager::GetInstance()->Push(pDropItem);
            continue;
        }
        ++it;
    }
}

void Map::updateTraps()
{
    for (auto *trap : m_traps)
    {
        if (trap)
            Trap::checkState(trap);
    }
}

void Map::updateTrailers()
{
    auto it = m_trailers.begin();
    while (it != m_trailers.end())
    {
        Trailer *pTrailer = *it;
        if (!pTrailer || Trailer::IsDestroyed(pTrailer))
        {
            it = m_trailers.erase(it);
            if (pTrailer)
                CPoolManager::GetInstance()->Push(pTrailer);
            continue;
        }
        ++it;
    }
}

void Map::SendStacksInMap(Player *player)
{
    if (!player) return;

    int8_t ConnId = Player::GetConnId(player);
    GameService *gs = GameService::GetInstance();
    Answer::NetPacket *packet = gs->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2722);
    if (!packet) return;
    appendStack(packet, &m_lstStack);
    packet->setSize(packet->getWOffset());
    int16_t GateIndex = Player::GetGateIndex(player);
    gs->sendPacketTo(ConnId, GateIndex, packet);
}

void Map::SendObjsInMap(Player *player)
{
    if (!player) return;

    int8_t ConnId = Player::GetConnId(player);
    int16_t GateIndex = Player::GetGateIndex(player);
    GameService *gs = GameService::GetInstance();

    Position playerPos = Unit::getCurrentTile(player);
    Position area = getAreaByPos(&playerPos);
    PositionVector outAreas;
    for (int32_t dy = -1; dy <= 1; ++dy)
        for (int32_t dx = -1; dx <= 1; ++dx)
            outAreas.push_back(Position(area.x + dx, area.y + dy));

    ObjList outObjs;
    getAllObjects(&outObjs);

    Answer::NetPacket *packet = gs->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2723);
    if (!packet) return;

    int32_t count = 0;
    uint32_t countPos = packet->getWOffset();
    packet->writeInt32(0);

    for (auto *pObj : outObjs)
    {
        if (!pObj) continue;
        if (inAreas(pObj, &outAreas))
        {
            StaticObj::appendInfo(pObj, packet);
            ++count;
        }
    }

    packet->writeInt32At(countPos, count);
    packet->setSize(packet->getWOffset());
    gs->sendPacketTo(ConnId, GateIndex, packet);
}

void Map::appendStack(Answer::NetPacket *packet, const PosList *posList)
{
    if (!packet || !posList) return;

    packet->writeInt32((int32_t)posList->size());
    for (auto &pos : *posList)
    {
        packet->writeInt16((int16_t)pos.x);
        packet->writeInt16((int16_t)pos.y);
    }
}

void Map::AddStack(const PosList *posList)
{
    if (!posList) return;

    for (auto &pos : *posList)
    {
        m_lstStack.push_back(pos);
    }

    GameService *gs = GameService::GetInstance();
    Answer::NetPacket *packet = gs->popNetpacket(0, Answer::PackType::PACK_DISPATCH, 0x2724);
    if (!packet) return;
    appendStack(packet, posList);
    packet->setSize(packet->getWOffset());
    broadcast(packet);
}

void Map::RemoveStack(const PosList *posList)
{
    if (!posList) return;

    for (auto &pos : *posList)
    {
        m_lstStack.remove(pos);
    }

    GameService *gs = GameService::GetInstance();
    Answer::NetPacket *packet = gs->popNetpacket(0, Answer::PackType::PACK_DISPATCH, 0x2725);
    if (!packet) return;
    appendStack(packet, posList);
    packet->setSize(packet->getWOffset());
    broadcast(packet);
}

void Map::removeStack(const Position *pos)
{
    if (!pos) return;
    m_lstStack.remove(*pos);
}

void Map::removeNpc(EntityId_t npcid)
{
    for (auto it = m_npcs.begin(); it != m_npcs.end(); ++it)
    {
        Npc *npc = *it;
        if (npc && Npc::GetId(npc) == npcid)
        {
            broadcastLeaveMap(npc);
            m_npcs.erase(it);
            return;
        }
    }
}

void Map::OnKillMonster(EntityId_t MonsterId)
{
    // Find and process monster death
    for (auto *monster : m_monsters)
    {
        if (monster && Unit::GetId(monster) == MonsterId)
        {
            Monster::OnKilled(monster);
            return;
        }
    }
}

void Map::AddContribution(int32_t Values, CURRENCY_CHANGE_REASON nOpWay)
{
    for (auto *pPlayer : m_players)
    {
        if (pPlayer)
            Player::AddCurrency(pPlayer, CURRENCY_TYPE::CURRENCY_CONTRIBUTION, Values, nOpWay, 0);
    }
}

void Map::delMonster(int32_t mid)
{
    for (auto *pMonster : m_monsters)
    {
        if (pMonster && Monster::getMid(pMonster) == mid)
            Monster::SetDestroy(pMonster);
    }
}

void Map::cleanActivityMonster(int32_t nActivityId)
{
    for (auto *pMonster : m_monsters)
    {
        if (pMonster && Monster::GetActivityId(pMonster) == nActivityId)
            Monster::SetDestroy(pMonster);
    }
}

void Map::OnStartGoblinRuQin(int32_t nActivityId, int32_t RefreshCount)
{
    CfgData *pCfgData = CfgData::GetInstance();
    const cfgGoblinTableData *GoblinTable = pCfgData->GetGoblinTableData(nActivityId);
    if (!GoblinTable) return;

    // Get refresh positions
    std::vector<Position> RefreshPos;
    for (auto &pos : GoblinTable->RefreshPos)
        RefreshPos.push_back(pos);

    std::random_shuffle(RefreshPos.begin(), RefreshPos.end());

    // Spawn goblins
    for (int32_t i = 0; i < RefreshCount && i < (int32_t)RefreshPos.size(); ++i)
    {
        GoblinAddMonster(nActivityId, GoblinTable->monsterId, GetMapId(),
            RefreshPos[i].x, RefreshPos[i].y);
    }
}

void Map::GoblinAddMonster(int32_t nActivityId, int32_t mid, int32_t mapId, int x, int y)
{
    CfgData *pCfgData = CfgData::GetInstance();
    const CfgMonster *pMonster = pCfgData->getMonster(mid);
    if (!pMonster) return;

    int32_t MonsterId = pCfgData->GetMapMonsterId();
    CfgMapMonster cfgmapmonster;
    memset(&cfgmapmonster, 0, sizeof(cfgmapmonster));
    cfgmapmonster.id = MonsterId;
    cfgmapmonster.mapid = mapId;
    cfgmapmonster.x = x;
    cfgmapmonster.y = y;

    CPoolManager *poolMgr = CPoolManager::GetInstance();
    Monster *monster = poolMgr->Pop<Monster>();
    if (monster)
    {
        AttrAddonList vAttrAddon;
        Monster::init(monster, pMonster, &cfgmapmonster, MonsterState::MS_STAND, &vAttrAddon);
        Monster::SetActivityId(monster, nActivityId);
        addMonster(monster, x, y);
    }
}

void Map::ActivityRefreshMonster(int32_t nActivityId, int32_t nTime, int32_t nIndex)
{
    CfgData *pCfgData = CfgData::GetInstance();
    RefreshMonsterCfg *v6 = pCfgData->GetRefreshMonsterCfg(nActivityId, nIndex);
    if (!v6) return;

    // Get positions
    std::vector<Position> PosVector;
    for (auto &pos : v6->RefreshPos)
        PosVector.push_back(pos);

    std::random_shuffle(PosVector.begin(), PosVector.end());

    int32_t nCount = v6->MonsterCount;
    int32_t nType = v6->nType;

    for (int32_t i = 0; i < nCount && i < (int32_t)PosVector.size(); ++i)
    {
        if (nType == 1) // Monster
        {
            const CfgMonster *pMonster = pCfgData->getMonster(v6->Mid);
            if (!pMonster) continue;

            int32_t MonsterId = pCfgData->GetMapMonsterId();
            CfgMapMonster cfgmapmonster;
            memset(&cfgmapmonster, 0, sizeof(cfgmapmonster));
            cfgmapmonster.id = MonsterId;
            cfgmapmonster.mapid = GetMapId();
            cfgmapmonster.x = PosVector[i].x;
            cfgmapmonster.y = PosVector[i].y;

            CPoolManager *poolMgr = CPoolManager::GetInstance();
            Monster *monster = poolMgr->Pop<Monster>();
            if (monster)
            {
                AttrAddonList vAttrAddon;
                Monster::init(monster, pMonster, &cfgmapmonster, MonsterState::MS_STAND, &vAttrAddon);
                Monster::SetActivityId(monster, nActivityId);
                addMonster(monster, PosVector[i].x, PosVector[i].y);
            }
        }
        else if (nType == 2) // Npc
        {
            const CfgNpc *pCfgNpc = pCfgData->getNpc(v6->Mid);
            if (!pCfgNpc) continue;

            Npc *npc = new Npc();
            if (npc)
            {
                Npc::init(npc, pCfgNpc, 0);
                addNpc(npc, PosVector[i].x, PosVector[i].y);
            }
        }
    }
}

bool Map::HaveUnit(int32_t x, int32_t y)
{
    for (auto *player : m_players)
    {
        if (player && Unit::getTileX(player) == x && Unit::getTileY(player) == y)
            return true;
    }
    for (auto *monster : m_monsters)
    {
        if (monster && Unit::isAlive(monster)
            && Unit::getTileX(monster) == x && Unit::getTileY(monster) == y)
            return true;
    }
    for (auto *plant : m_plants)
    {
        if (plant && Unit::getTileX(plant) == x && Unit::getTileY(plant) == y)
            return true;
    }
    return false;
}

bool Map::HaveMonster(int32_t x, int32_t y)
{
    for (auto *monster : m_monsters)
    {
        if (monster && Unit::getTileX(monster) == x && Unit::getTileY(monster) == y)
            return true;
    }
    return false;
}

Position Map::GetDirTileByDistance(Position CurPos, int32_t Direc, int32_t distance)
{
    Direction dir = (Direction)Direc;
    Position result = CurPos;
    for (int32_t i = 0; i < distance; ++i)
    {
        Position next = getAroundTile(result.x, result.y, dir);
        if (next.x == result.x && next.y == result.y)
            break;
        result = next;
    }
    return result;
}

Position Map::MoveOneTile(Position CurPos, int32_t Direc)
{
    Direction dir = (Direction)Direc;
    Position pos = getAroundTile(CurPos.x, CurPos.y, dir);

    if (pos.x != CurPos.x || pos.y != CurPos.y)
    {
        if (isWalkablePosition(pos.x, pos.y) && !HaveUnit(pos.x, pos.y))
            return pos;
    }

    // Try nearby directions
    int32_t dirs[] = { (Direc + 2) % 8, (Direc + 6) % 8, (Direc + 1) % 8, (Direc + 7) % 8 };
    for (int32_t d : dirs)
    {
        pos = getAroundTile(CurPos.x, CurPos.y, (Direction)d);
        if (pos.x != CurPos.x || pos.y != CurPos.y)
        {
            if (isWalkablePosition(pos.x, pos.y) && !HaveUnit(pos.x, pos.y))
                return pos;
        }
    }

    return CurPos;
}

void Map::AllPlayerBackCity()
{
    PlayerList playerListTmp = m_players;
    for (auto *player : playerListTmp)
    {
        if (player)
            Player::backToCity(player);
    }
}

int32_t Map::GetMapPlayerSize()
{
    return (int32_t)m_players.size();
}

int32_t Map::GetAliveBossCount(int32_t nLevel)
{
    int32_t Count = 0;
    for (auto *monster : m_monsters)
    {
        if (monster && Unit::isAlive(monster) && Monster::GetBossLevel(monster) >= nLevel)
            ++Count;
    }
    return Count;
}

int32_t Map::GetMapPlayerAliveSize()
{
    int32_t Count = 0;
    for (auto *player : m_players)
    {
        if (player && Unit::isAlive(player))
            ++Count;
    }
    return Count;
}

bool Map::NearNpc(int32_t NpcId, int32_t x, int32_t y, int32_t Diff)
{
    for (auto *npc : m_npcs)
    {
        if (!npc) continue;
        if (Npc::GetNpcId(npc) != NpcId) continue;

        int32_t nx = Unit::getTileX(npc);
        int32_t ny = Unit::getTileY(npc);
        int32_t dx = abs(nx - x);
        int32_t dy = abs(ny - y);
        if (dx <= Diff && dy <= Diff)
            return true;
    }
    return false;
}
