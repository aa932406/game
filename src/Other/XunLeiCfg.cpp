#include "Other/XunLeiCfg.h"

#include <cstring>
#include <algorithm>

XunLeiCfg::XunLeiCfg(const XunLeiCfg& a2)
{
    this->nIndex = a2.nIndex;
    this->nType = a2.nType;
    this->vRewards = a2.vRewards;
    this->nCondition = a2.nCondition;
}

