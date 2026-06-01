#include "Other/VipEQuipPosSuit.h"

#include <cstring>
#include <algorithm>

VipEQuipPosSuit::VipEQuipPosSuit(const VipEQuipPosSuit *const a2)
{
    this->nIndex = a2->nIndex;
    this->nLevel = a2->nLevel;
    std::list<AddAttribute>::list(&this->lAddAttrs, &a2->lAddAttrs);
    this->nTalentId = a2->nTalentId;
    this->TalentLevel = a2->TalentLevel;
}

