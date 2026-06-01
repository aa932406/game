#include "Other/ShopGoods.h"

#include <cstring>
#include <algorithm>

ShopGoods::ShopGoods(const ShopGoods *const a2)
{
    std::vector<MemChrBag>::vector(&this->vItem, &a2->vItem);
    this->nPrice = a2->nPrice;
}

