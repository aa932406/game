#include "Other/ShiZhuangSuitCfg.h"

#include <cstring>
#include <algorithm>

ShiZhuangSuitCfg::ShiZhuangSuitCfg(const ShiZhuangSuitCfg *const a2)
{
    this->nSuidId = a2->nSuidId;
    this->nCount = a2->nCount;
    std::list<ShiZhuangSuitInfo>::list(&this->lInfo, &a2->lInfo);
}

