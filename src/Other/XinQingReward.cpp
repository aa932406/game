#include "Other/XinQingReward.h"

#include <cstring>
#include <algorithm>

XinQingReward::XinQingReward(const XinQingReward *const a2)
{
    this->GongGaoId = a2->GongGaoId;
    std::vector<MemChrBag>::vector(&this->Items, &a2->Items);
}

