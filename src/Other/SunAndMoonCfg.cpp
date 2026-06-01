#include "Other/SunAndMoonCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

SunAndMoonCfg::SunAndMoonCfg()
    : nLevel(0), nGetItemTimes(0), nSunTelentId(0), nSunTelentLevel(0), nMoonTelentId(0)
{
}

SunAndMoonCfg::~SunAndMoonCfg()
{
}

SunAndMoonCfg::SunAndMoonCfg(const SunAndMoonCfg& other)
    : nLevel(other.nLevel)
    , lConstItems(other.lConstItems)
    , lAttr(other.lAttr)
    , nGetItemTimes(other.nGetItemTimes)
    , nSunTelentId(other.nSunTelentId)
    , nSunTelentLevel(other.nSunTelentLevel)
    , nMoonTelentId(other.nMoonTelentId)
{
}
