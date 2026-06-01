#include "Other/LuDaShiVip.h"

LuDaShiVip::LuDaShiVip()
    : nIndex(0)
    , nType(0)
    , nCondition(0)
    , nMaxCondition(0)
    , nVipType(0)
{
}

LuDaShiVip::LuDaShiVip(const LuDaShiVip& a2)
    : nIndex(a2.nIndex)
    , nType(a2.nType)
    , Rewards(a2.Rewards)
    , nCondition(a2.nCondition)
    , nMaxCondition(a2.nMaxCondition)
    , nVipType(a2.nVipType)
{
}
