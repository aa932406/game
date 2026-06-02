#include "Other/ShopGoods.h"

#include <cstring>
#include <algorithm>

ShopGoods::ShopGoods(const ShopGoods& a2)
{
    this->vItem = a2.vItem;
    this->nPrice = a2.nPrice;
}

