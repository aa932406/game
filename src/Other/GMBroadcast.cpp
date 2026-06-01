#include "Other/GMBroadcast.h"

#include <cstring>
#include <algorithm>

GMBroadcast::GMBroadcast()
    : nId(0)
    , nType(0)
    , nStartTime(0)
    , nInterval(0)
    , nTimes(0)
    , nLastTime(0)
    , nCount(0)
{
}

GMBroadcast::GMBroadcast(const GMBroadcast& other)
    : nId(other.nId)
    , nType(other.nType)
    , strText(other.strText)
    , nStartTime(other.nStartTime)
    , nInterval(other.nInterval)
    , nTimes(other.nTimes)
    , nLastTime(other.nLastTime)
    , nCount(other.nCount)
{
}