#include "Other/VipCfg.h"
#include "Common/CommonTypes.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

VipCfg::VipCfg()
    : VipLevel(0), NeedVipExp(0), Retroactive(0), Luck1(0)
{
}

VipCfg::~VipCfg()
{
}

VipCfg::VipCfg(const VipCfg& other)
    : VipLevel(other.VipLevel)
    , NeedVipExp(other.NeedVipExp)
    , Item(other.Item)
    , DayReward(other.DayReward)
    , AtttVector(other.AtttVector)
    , Retroactive(other.Retroactive)
    , Luck1(other.Luck1)
{
}
