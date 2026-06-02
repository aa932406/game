#include "Other/SuperTeHuiCfg.h"

#include <cstring>
#include <algorithm>

SuperTeHuiCfg::SuperTeHuiCfg(const SuperTeHuiCfg& a2)
{
    this->nIndex = a2.nIndex;
    this->nNeedVip = a2.nNeedVip;
    this->Items = a2.Items;
    this->nPrice = a2.nPrice;
    this->nGongGaoId = a2.nGongGaoId;
}

