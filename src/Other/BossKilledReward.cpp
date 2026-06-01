#include "Other/BossKilledReward.h"
#include "Common/CommonTypes.h"

#include <cstring>
#include <algorithm>

BossKilledReward::BossKilledReward()
    : nType(0)
{
}

BossKilledReward::~BossKilledReward()
{
}

BossKilledReward::BossKilledReward(const BossKilledReward& other)
    : nType(other.nType)
    , BossList(other.BossList)
    , Rewars(other.Rewars)
{
}
