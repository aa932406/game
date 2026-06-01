#include "Other/SevenTouZi.h"

#include <cstring>
#include <algorithm>

SevenTouZi::SevenTouZi(const SevenTouZi *const a2)
{
    this->nId = a2->nId;
    this->nType = a2->nType;
    this->nCondition = a2->nCondition;
    std::vector<MemChrBag>::vector(&this->vItem, &a2->vItem);
}

