#include "Other/Speed360Reward.h"

#include <cstring>
#include <algorithm>

Speed360Reward::Speed360Reward(const Speed360Reward *const a2)
{
    this->StartTime = a2->StartTime;
    this->EndTime = a2->EndTime;
    std::vector<MemChrBag>::vector(&this->Rewards, &a2->Rewards);
}

