#include "Other/cfgGoblinData.h"
#include "Utility/Position.h"

#include <cstring>
#include <algorithm>

cfgGoblinData::cfgGoblinData()
    : m_mapId(0), m_nIndex(0), m_nRefreshTime(0)
{
}

cfgGoblinData::~cfgGoblinData()
{
}

cfgGoblinData::cfgGoblinData(const cfgGoblinData& other)
    : m_nIndex(other.m_nIndex)
    , m_nRefreshTime(other.m_nRefreshTime)
    , m_RefreshMonsters(other.m_RefreshMonsters)
    , m_mapId(other.m_mapId)
    , m_RevivePosVector(other.m_RevivePosVector)
{
}
