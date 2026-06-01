#include "Other/MingGeDrawRate.h"

#include <cstring>
#include <algorithm>

MingGeDrawRate::MingGeDrawRate()
    : nMaxRate(0)
{
    nItemRate.clear();
}

MingGeDrawRate::MingGeDrawRate(const MingGeDrawRate& other)
    : nItemRate(other.nItemRate)
    , nMaxRate(other.nMaxRate)
{
}
