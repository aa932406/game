#include "Other/RongHeCfg.h"

#include <cstring>
#include <algorithm>

RongHeCfg::RongHeCfg(const RongHeCfg *const a2)
{
    this->nId = a2->nId;
    this->nCostItem = a2->nCostItem;
    std::list<RongHeItem>::list(&this->lRongHeItemList, &a2->lRongHeItemList);
    this->nMaxRate = a2->nMaxRate;
    this->nMaxRate2 = a2->nMaxRate2;
}

