#include "Other/KillerInfo.h"

KillerInfo::KillerInfo()
    : nCharId(0)
    , nTime(0)
{
}

KillerInfo::KillerInfo(const KillerInfo& a2)
    : nCharId(a2.nCharId)
    , strName(a2.strName)
    , nTime(a2.nTime)
{
}
