#include "Other/LevelRefinCfg.h"

#include <cstring>
#include <algorithm>

LevelRefinCfg::LevelRefinCfg()
    : nLevel(0), nLimit(0), nGongGaoId(0)
{
}

LevelRefinCfg::~LevelRefinCfg()
{
}

LevelRefinCfg::LevelRefinCfg(const LevelRefinCfg& other)
    : nLevel(other.nLevel)
    , vNeedGold(other.vNeedGold)
    , nLevelUp(other.nLevelUp)
    , nLimit(other.nLimit)
    , nGongGaoId(other.nGongGaoId)
{
}
