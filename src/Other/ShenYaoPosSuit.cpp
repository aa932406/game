#include "Other/ShenYaoPosSuit.h"

#include <cstring>
#include <algorithm>

ShenYaoPosSuit::ShenYaoPosSuit(const ShenYaoPosSuit *const a2)
{
    this->nLevel = a2->nLevel;
    std::vector<AttrAddon>::vector(&this->vAttrAddon, &a2->vAttrAddon);
}

