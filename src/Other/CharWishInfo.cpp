#include "Other/CharWishInfo.h"

#include <cstring>
#include <algorithm>

CharWishInfo::CharWishInfo()
    : nId(0)
    , nItemId(0)
    , nStartTime(0)
    , nGetReward(0)
{
}

CharWishInfo::CharWishInfo(const CharWishInfo& other)
    : nId(other.nId)
    , nItemId(other.nItemId)
    , nStartTime(other.nStartTime)
    , nGetReward(other.nGetReward)
    , strInfo(other.strInfo)
{
}
