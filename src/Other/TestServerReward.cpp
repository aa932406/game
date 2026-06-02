#include "Other/TestServerReward.h"

#include <cstring>
#include <algorithm>

TestServerReward::TestServerReward(const TestServerReward& a2)
{
    this->nId = a2.nId;
    this->nType = a2.nType;
    this->nParm = a2.nParm;
    this->vItems = a2.vItems;
    this->nGongId = a2.nGongId;
}

