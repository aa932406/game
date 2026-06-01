#include "Other/PlatformDaTing.h"

#include <cstring>
#include <algorithm>

PlatformDaTing::PlatformDaTing()
    : nId(0)
    , nType(0)
    , nParam(0)
    , nIcon(0)
    , nParam2(0)
{
}

PlatformDaTing::~PlatformDaTing()
{
}

PlatformDaTing::PlatformDaTing(const PlatformDaTing& other)
    : Platform(other.Platform)
    , nId(other.nId)
    , nType(other.nType)
    , nParam(other.nParam)
    , vItems(other.vItems)
    , nIcon(other.nIcon)
    , nParam2(other.nParam2)
{
}
