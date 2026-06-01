#include "Other/ZeroBuyPetCfg.h"

#include <cstring>
#include <algorithm>

ZeroBuyPetCfg::ZeroBuyPetCfg()
    : nTime(0)
    , nGold(0)
    , nMailId(0)
{
    cItems.clear();
}

ZeroBuyPetCfg::ZeroBuyPetCfg(const ZeroBuyPetCfg& other)
    : cItems(other.cItems)
    , nTime(other.nTime)
    , nGold(other.nGold)
    , nMailId(other.nMailId)
{
}

bool ZeroBuyPetCfg::IsEmpty() const
{
    return !nTime || cItems.empty() || !nGold;
}
