#include "Other/StaticObj.h"
#include "Game/Map.h"

StaticObj::StaticObj()
    : m_entityType(static_cast<EntityType>(0))
    , m_currentPos(0, 0)
    , m_currentTile(0, 0)
    , m_pMap(nullptr)
{
}

StaticObj::StaticObj(EntityType entitytype)
    : m_entityType(entitytype)
    , m_currentPos(0, 0)
    , m_currentTile(0, 0)
    , m_pMap(nullptr)
{
}

StaticObj::~StaticObj()
{
}

void StaticObj::reset()
{
    m_pMap = nullptr;
    m_currentTile = Position(0, 0);
    m_currentPos = Position(0, 0);
}

int32_t StaticObj::getMapId() const
{
    if (m_pMap) {
        // return m_pMap->GetMapId();
    }
    return 0;
}

int32_t StaticObj::getMapType() const
{
    if (m_pMap) {
        // return m_pMap->GetType();
    }
    return static_cast<int32_t>(MapType::MT_NORMAL);
}

bool StaticObj::InDungeon() const
{
    if (m_pMap) {
        return getMapType() == static_cast<int32_t>(MapType::MT_DUNGEON);
    }
    return false;
}

bool StaticObj::InActivity() const
{
    if (m_pMap) {
        return getMapType() == static_cast<int32_t>(MapType::MT_ACTIVITY);
    }
    return false;
}

bool StaticObj::InPrison() const
{
    if (m_pMap) {
        return getMapType() == static_cast<int32_t>(MapType::MT_PRISON);
    }
    return false;
}

int32_t StaticObj::GetRunnerId() const
{
    if (m_pMap) {
        // return m_pMap->GetRunnerId();
    }
    return 0;
}

bool StaticObj::IsSpecialBossMap() const
{
    if (m_pMap) {
        return getMapType() == static_cast<int32_t>(MapType::MT_SPECIAL_BOSS);
    }
    return false;
}

bool StaticObj::IsCycleTower() const
{
    if (m_pMap) {
        return getMapType() == static_cast<int32_t>(MapType::MT_CYCLE_TOWER);
    }
    return false;
}

bool StaticObj::IsKuangDong() const
{
    if (m_pMap) {
        return getMapType() == static_cast<int32_t>(MapType::MT_KUANG_DONG);
    }
    return false;
}

bool StaticObj::IsTerritory() const
{
    if (m_pMap) {
        return getMapType() == static_cast<int32_t>(MapType::MT_TERRITORY);
    }
    return false;
}