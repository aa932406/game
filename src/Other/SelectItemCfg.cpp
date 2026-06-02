#include "Other/SelectItemCfg.h"

#include <cstring>
#include <algorithm>

SelectItemCfg::SelectItemCfg(const SelectItemCfg& a2)
{
    this->nId = a2.nId;
    this->vItem = a2.vItem;
}

