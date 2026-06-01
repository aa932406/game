#include "Other/TianLingCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

TianLingCfg::TianLingCfg()
    : Level(0), GongGaoId(0), CostMoney(0), CostCoin(0)
{
}

TianLingCfg::~TianLingCfg()
{
}

TianLingCfg::TianLingCfg(const TianLingCfg& other)
    : Level(other.Level)
    , CostMoney(other.CostMoney)
    , CostItems(other.CostItems)
    , AttrList(other.AttrList)
    , GongGaoId(other.GongGaoId)
    , CostCoin(other.CostCoin)
{
}
