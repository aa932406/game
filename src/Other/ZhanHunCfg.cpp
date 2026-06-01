#include "Other/ZhanHunCfg.h"

#include <cstring>
#include <algorithm>

ZhanHunCfg::ZhanHunCfg(const ZhanHunCfg *const a2)
{
    this->nLevel = a2->nLevel;
    this->nMoney = a2->nMoney;
    this->nSyb = a2->nSyb;
    std::list<AddAttribute>::list(&this->lAddAttrs, &a2->lAddAttrs);
    this->nGongGaoId = a2->nGongGaoId;
}

