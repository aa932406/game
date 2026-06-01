#include "Other/QiQingCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

QiQingCfg::QiQingCfg()
    : nLevel(0), GongGaoId(0)
{
}

QiQingCfg::~QiQingCfg()
{
}

QiQingCfg::QiQingCfg(const QiQingCfg& other)
    : nLevel(other.nLevel)
    , CostItems(other.CostItems)
    , Attr(other.Attr)
    , GongGaoId(other.GongGaoId)
{
}
