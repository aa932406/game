#include "Other/TreasureMapCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

TreasureMapCfg::TreasureMapCfg()
    : m_ItemId(0), m_MaxProbability(0)
{
}

TreasureMapCfg::~TreasureMapCfg()
{
}

TreasureMapCfg::TreasureMapCfg(const TreasureMapCfg& other)
    : m_ItemId(other.m_ItemId)
    , m_EventRate(other.m_EventRate)
    , m_MaxProbability(other.m_MaxProbability)
    , m_MaxPosVector(other.m_MaxPosVector)
{
}
