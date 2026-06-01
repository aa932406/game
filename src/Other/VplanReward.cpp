#include "Other/VplanReward.h"
#include "Common/CommonTypes.h"

#include <cstring>
#include <algorithm>

VplanReward::VplanReward()
    : Index(0)
{
}

VplanReward::~VplanReward()
{
}

VplanReward::VplanReward(const VplanReward& other)
    : Index(other.Index)
    , Reward(other.Reward)
    , YearVipReward(other.YearVipReward)
{
}
