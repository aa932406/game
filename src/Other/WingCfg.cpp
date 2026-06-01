#include "Other/WingCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

WingCfg::WingCfg()
    : Level(0), StartPoints(0), SuccessPoints(0), MaxPoints(0), Rate(0), FailAddPoints(0)
{
}

WingCfg::~WingCfg()
{
}

WingCfg::WingCfg(const WingCfg& other)
    : Level(other.Level)
    , ConstItems(other.ConstItems)
    , StartPoints(other.StartPoints)
    , SuccessPoints(other.SuccessPoints)
    , MaxPoints(other.MaxPoints)
    , Rate(other.Rate)
    , FailAddPoints(other.FailAddPoints)
    , AddonVector(other.AddonVector)
{
}
