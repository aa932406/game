#include "Other/MiniClientCfg.h"

#include <cstring>
#include <algorithm>

MiniClientCfg::MiniClientCfg()
    : nId(0)
    , nType(0)
    , nParam(0)
    , nIcon(0)
    , nParam2(0)
{
}

MiniClientCfg::~MiniClientCfg()
{
}

MiniClientCfg::MiniClientCfg(const MiniClientCfg& other)
    : Platform(other.Platform)
    , nId(other.nId)
    , nType(other.nType)
    , nParam(other.nParam)
    , vItems(other.vItems)
    , nIcon(other.nIcon)
    , nParam2(other.nParam2)
{
}