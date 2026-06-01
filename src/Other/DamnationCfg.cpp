#include "Other/DamnationCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

DamnationCfg::DamnationCfg()
    : Level(0), CostMoney(0), Probability(0), HpPecent(0), GongGaoId(0)
{
}

DamnationCfg::~DamnationCfg()
{
}

DamnationCfg::DamnationCfg(const DamnationCfg& other)
    : Level(other.Level)
    , CostItems(other.CostItems)
    , CostMoney(other.CostMoney)
    , AttrList(other.AttrList)
    , Probability(other.Probability)
    , HpPecent(other.HpPecent)
    , GongGaoId(other.GongGaoId)
{
}
