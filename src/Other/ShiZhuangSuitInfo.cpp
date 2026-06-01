#include "Other/ShiZhuangSuitInfo.h"

#include <cstring>
#include <algorithm>

ShiZhuangSuitInfo::ShiZhuangSuitInfo(const ShiZhuangSuitInfo *const a2)
{
    this->nLevel = a2->nLevel;
    this->nSuitLevel = a2->nSuitLevel;
    std::list<AddAttribute>::list(&this->vAttr, &a2->vAttr);
}

