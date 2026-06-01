#include "Other/DieRecord.h"

#include <cstring>
#include <algorithm>

DieRecord::DieRecord()
    : Cid(0)
    , KillTime(0)
    , MapId(0)
    , MonsterId(0)
{
    Name.clear();
}

DieRecord::DieRecord(const DieRecord& other)
    : Cid(other.Cid)
    , KillTime(other.KillTime)
    , MapId(other.MapId)
    , MonsterId(other.MonsterId)
    , Name(other.Name)
{
}
