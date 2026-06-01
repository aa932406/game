#include "Other/UltimateChallengeCfg.h"

#include <cstring>
#include <algorithm>

UltimateChallengeCfg::UltimateChallengeCfg()
{
}

UltimateChallengeCfg::~UltimateChallengeCfg()
{
}

UltimateChallengeCfg::UltimateChallengeCfg(const UltimateChallengeCfg& other)
    : GetItems(other.GetItems)
    , DropRateList(other.DropRateList)
{
}
