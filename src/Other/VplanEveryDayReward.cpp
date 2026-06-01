#include "Other/VplanEveryDayReward.h"

#include <cstring>
#include <algorithm>

VplanEveryDayReward::VplanEveryDayReward()
{
}

VplanEveryDayReward::~VplanEveryDayReward()
{
}

VplanEveryDayReward::VplanEveryDayReward(const VplanEveryDayReward& other)
    : Reward(other.Reward)
    , YearVipReward(other.YearVipReward)
{
}
