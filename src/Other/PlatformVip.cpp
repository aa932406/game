#include "Other/PlatformVip.h"

#include <cstring>
#include <algorithm>

PlatformVip::PlatformVip()
    : nId(0)
    , nNeedVipLevel(0)
    , nIcon(0)
{
}

PlatformVip::~PlatformVip()
{
}

PlatformVip::PlatformVip(const PlatformVip& other)
    : sPlatform(other.sPlatform)
    , nId(other.nId)
    , nNeedVipLevel(other.nNeedVipLevel)
    , vItem(other.vItem)
    , nIcon(other.nIcon)
{
}
