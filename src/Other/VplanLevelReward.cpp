#include "Other/VplanLevelReward.h"

#include <cstring>
#include <algorithm>

VplanLevelReward::VplanLevelReward(const VplanLevelReward *const a2)
{
    this->Index = a2->Index;
    this->Level = a2->Level;
    std::vector<MemChrBag>::vector(&this->Reward, &a2->Reward);
}

