#include "Other/DropRecord.h"

DropRecord::DropRecord()
    : nCharId(0)
    , nMapId(0)
    , nMid(0)
    , nRecord(0)
    , nTime(0)
    , nSpecial(0)
{
}

DropRecord::DropRecord(const DropRecord& a2)
    : strName(a2.strName)
    , nCharId(a2.nCharId)
    , nMapId(a2.nMapId)
    , nMid(a2.nMid)
    , nRecord(a2.nRecord)
    , nTime(a2.nTime)
    , nSpecial(a2.nSpecial)
{
}
