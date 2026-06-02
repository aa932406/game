#include "Other/RongHeCfg.h"

#include <cstring>
#include <algorithm>

RongHeCfg::RongHeCfg(const RongHeCfg& a2)
{
    this->nId = a2.nId;
    this->nCostItem = a2.nCostItem;
    this->lRongHeItemList = a2.lRongHeItemList;
    this->nMaxRate = a2.nMaxRate;
    this->nMaxRate2 = a2.nMaxRate2;
}

