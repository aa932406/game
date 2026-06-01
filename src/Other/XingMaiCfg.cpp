#include "Other/XingMaiCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

XingMaiCfg::XingMaiCfg()
    : Level(0), PlayerLevel(0), GongGaoId(0), NeedBossScore(0), BossScoreLimit(0)
{
}

XingMaiCfg::~XingMaiCfg()
{
}

XingMaiCfg::XingMaiCfg(const XingMaiCfg& other)
    : Level(other.Level)
    , PlayerLevel(other.PlayerLevel)
    , CostItems(other.CostItems)
    , GongGaoId(other.GongGaoId)
    , AttrList(other.AttrList)
    , NeedBossScore(other.NeedBossScore)
    , BossScoreLimit(other.BossScoreLimit)
{
}
