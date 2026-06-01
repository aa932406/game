#include "Other/TestServerReward.h"

#include <cstring>
#include <algorithm>

TestServerReward::TestServerReward(const TestServerReward *const a2)
{
    this->nId = a2->nId;
    this->nType = a2->nType;
    this->nParm = a2->nParm;
    std::vector<MemChrBag>::vector(&this->vItems, &a2->vItems);
    this->nGongId = a2->nGongId;
}

