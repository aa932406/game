#include "Other/ShouHuRefinishingCfg.h"

#include <cstring>
#include <algorithm>

ShouHuRefinishingCfg::~ShouHuRefinishingCfg()
{
    std::list<AddAttribute>::~list(&this->lAttrList);
    std::list<ItemData>::~list(&this->lCostList);
}
