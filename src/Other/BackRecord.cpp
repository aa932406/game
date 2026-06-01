#include "Other/BackRecord.h"

BackRecord::BackRecord()
    : nId(0)
    , nType(0)
    , nTime(0)
{
}

BackRecord::BackRecord(const BackRecord& a2)
    : nId(a2.nId)
    , nType(a2.nType)
    , nTime(a2.nTime)
    , name(a2.name)
{
}
