#include "Other/ShiZhuangSuitInfo.h"

#include <cstring>
#include <algorithm>

ShiZhuangSuitInfo::ShiZhuangSuitInfo(const ShiZhuangSuitInfo& a2)
{
    this->nLevel = a2.nLevel;
    this->nSuitLevel = a2.nSuitLevel;
    this->vAttr = a2.vAttr;
}

