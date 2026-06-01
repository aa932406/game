#include "Other/RefreshMonsterCfg.h"

#include "Utility/Position.h"

#include <cstring>
#include <algorithm>

RefreshMonsterCfg::RefreshMonsterCfg(const RefreshMonsterCfg *const a2)
{
    this->nIndex = a2->nIndex;
    this->nType = a2->nType;
    this->nId = a2->nId;
    this->nCount = a2->nCount;
    this->nStartTime = a2->nStartTime;
    this->nDiffTime = a2->nDiffTime;
    std::vector<Position>::vector(&this->vRevivePosVector, &a2->vRevivePosVector);
}

