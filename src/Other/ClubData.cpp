#include "Other/ClubData.h"

ClubData::ClubData()
    : nCid(0)
    , nVipLevel(0)
    , nVip7Time(0)
    , nVip10Time(0)
{
}

ClubData::ClubData(const ClubData& a2)
    : nCid(a2.nCid)
    , strName(a2.strName)
    , nVipLevel(a2.nVipLevel)
    , nVip7Time(a2.nVip7Time)
    , nVip10Time(a2.nVip10Time)
{
}
