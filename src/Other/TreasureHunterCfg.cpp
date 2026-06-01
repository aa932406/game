#include "Other/TreasureHunterCfg.h"

#include <cstring>
#include <algorithm>

TreasureHunterCfg::TreasureHunterCfg(const TreasureHunterCfg *const a2)
{
    this->nId = a2->nId;
    std::vector<MemChrBag>::vector(&this->Items, &a2->Items);
    this->nRequire = a2->nRequire;
    this->GongGaoId = a2->GongGaoId;
    this->nType = a2->nType;
}

