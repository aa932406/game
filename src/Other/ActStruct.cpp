#include "Other/ActStruct.h"

#include <cstring>
#include <algorithm>

ActStruct::ActStruct()
    : nSid(0), Time(0), connid(0)
{
}

ActStruct::~ActStruct()
{
}

ActStruct::ActStruct(const ActStruct& other)
    : name(other.name)
    , passport(other.passport)
    , nSid(other.nSid)
    , Time(other.Time)
    , connid(other.connid)
{
}
