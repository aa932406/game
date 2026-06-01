#include "Other/XunLeiCfg.h"

#include <cstring>
#include <algorithm>

XunLeiCfg::XunLeiCfg(const XunLeiCfg *const a2)
{
    this->nIndex = a2->nIndex;
    this->nType = a2->nType;
    std::vector<MemChrBag>::vector(&this->vRewards, &a2->vRewards);
    this->nCondition = a2->nCondition;
}

