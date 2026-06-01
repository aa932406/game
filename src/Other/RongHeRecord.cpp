#include "Other/RongHeRecord.h"

#include <cstring>
#include <algorithm>

RongHeRecord::RongHeRecord(const RongHeRecord *const a2)
{
    this->nCid = a2->nCid;
    std::string::string(&this->strName, &a2->strName);
    this->nCostId = a2->nCostId;
    this->nGiveId = a2->nGiveId;
    this->nSuccess = a2->nSuccess;
    this->nTime = a2->nTime;
}

