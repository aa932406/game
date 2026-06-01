#include "Other/Wan360Reward.h"

#include <cstring>
#include <algorithm>

Wan360Reward::Wan360Reward(const Wan360Reward *const a2)
{
    this->Index = a2->Index;
    std::vector<MemChrBag>::vector(&this->Items, &a2->Items);
}

