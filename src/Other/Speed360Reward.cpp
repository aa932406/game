#include "Other/Speed360Reward.h"

#include <cstring>
#include <algorithm>

Speed360Reward::Speed360Reward(const Speed360Reward& a2)
{
    this->StartTime = a2.StartTime;
    this->EndTime = a2.EndTime;
    this->Rewards = a2.Rewards;
}

