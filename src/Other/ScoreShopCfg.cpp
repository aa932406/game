#include "Other/ScoreShopCfg.h"

#include <cstring>
#include <algorithm>

ScoreShopCfg::ScoreShopCfg(const ScoreShopCfg *const a2)
{
    this->Index = a2->Index;
    this->PlayerLevel = a2->PlayerLevel;
    this->LimitType = a2->LimitType;
    this->LimitCount = a2->LimitCount;
    this->Item = a2->Item;
    this->CostType = a2->CostType;
    this->CostValue = a2->CostValue;
}

