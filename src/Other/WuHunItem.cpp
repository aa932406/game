#include "Other/WuHunItem.h"

#include <cstring>
#include <algorithm>

WuHunItem::WuHunItem(const WuHunItem *const a2)
{
    this->nId = a2->nId;
    this->nLevel = a2->nLevel;
    this->nType = a2->nType;
    this->nQuality = a2->nQuality;
    this->nNeedQuality = a2->nNeedQuality;
    std::list<AddAttribute>::list(&this->lAttrList, &a2->lAttrList);
    this->nTalentId = a2->nTalentId;
}

