#include "Other/BossKilledRewardInfo.h"

#include <cstring>
#include <algorithm>

bool BossKilledRewardInfo::CanGetReward()
{
    if (IsGetReward > 0)
        return false;
    return true;
}

bool BossKilledRewardInfo::SetBossKilled(int32_t BossId)
{
    BossKeilled.push_back(BossId);
    return true;
}
