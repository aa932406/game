#include "Other/WinRefiningCfg.h"

#include <cstring>
#include <algorithm>

WinRefiningCfg::WinRefiningCfg(const WinRefiningCfg *const a2)
{
    this->nSlot = a2->nSlot;
    this->nLevel = a2->nLevel;
    this->Attr = a2->Attr;
    std::list<ItemData>::list(&this->lCostList, &a2->lCostList);
    this->SuitId = a2->SuitId;
    this->nConstMoney = a2->nConstMoney;
    this->GongGaoId = a2->GongGaoId;
}

