#include "Other/BackLimitInfo.h"

BackLimitInfo::BackLimitInfo()
    : nId(0)
    , nCount(0)
{
}

BackLimitInfo::BackLimitInfo(const BackLimitInfo& a2)
    : nId(a2.nId)
    , nCount(a2.nCount)
    , sLastName(a2.sLastName)
{
}
