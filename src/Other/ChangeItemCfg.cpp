#include "Other/ChangeItemCfg.h"
#include "Common/CommonTypes.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

ChangeItemCfg::ChangeItemCfg()
    : nCostGold(0), Times(0)
{
}

ChangeItemCfg::~ChangeItemCfg()
{
}

ChangeItemCfg::ChangeItemCfg(const ChangeItemCfg& other)
    : lCostItem(other.lCostItem)
    , nCostGold(other.nCostGold)
    , GetItems(other.GetItems)
    , Times(other.Times)
{
}
