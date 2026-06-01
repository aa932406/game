#include "Other/CVipClubLuckyDrop.h"

CVipClubLuckyDrop::CVipClubLuckyDrop()
    : nCondition(0)
    , nLevel(0)
    , nRand(0)
{
}

CVipClubLuckyDrop::CVipClubLuckyDrop(const CVipClubLuckyDrop& a2)
    : nCondition(a2.nCondition)
    , nLevel(a2.nLevel)
    , lRateItemList(a2.lRateItemList)
    , nRand(a2.nRand)
{
}
