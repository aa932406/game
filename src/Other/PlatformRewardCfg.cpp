#include "Other/PlatformRewardCfg.h"

#include <cstring>
#include <algorithm>

PlatformRewardCfg::PlatformRewardCfg()
    : nId(0)
    , nIcon(0)
    , nIsAllShow(0)
{
}

PlatformRewardCfg::~PlatformRewardCfg()
{
}

PlatformRewardCfg::PlatformRewardCfg(const PlatformRewardCfg& other)
    : nId(other.nId)
    , vItem(other.vItem)
    , nIcon(other.nIcon)
    , sPlatform(other.sPlatform)
    , nIsAllShow(other.nIsAllShow)
{
}
