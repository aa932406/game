#include "Other/ShangRenCfg.h"

#include <cstring>
#include <algorithm>

ShangRenCfg::ShangRenCfg(const ShangRenCfg *const a2)
{
    this->nId = a2->nId;
    std::vector<MemChrBag>::vector(&this->vItem, &a2->vItem);
    this->nPrice = a2->nPrice;
}

