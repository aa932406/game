#include "Other/SelectItemCfg.h"

#include <cstring>
#include <algorithm>

SelectItemCfg::SelectItemCfg(const SelectItemCfg *const a2)
{
    this->nId = a2->nId;
    std::vector<MemChrBag>::vector(&this->vItem, &a2->vItem);
}

