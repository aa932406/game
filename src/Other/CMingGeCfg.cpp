#include "Other/CMingGeCfg.h"

CMingGeCfg::CMingGeCfg()
    : nId(0)
    , nType(0)
    , nCanPickup(0)
    , nLevel(0)
    , nQuality(0)
    , nDecExp(0)
{
}

CMingGeCfg::CMingGeCfg(const CMingGeCfg& a2)
    : nId(a2.nId)
    , lAttrList(a2.lAttrList)
    , nType(a2.nType)
    , nCanPickup(a2.nCanPickup)
    , nLevel(a2.nLevel)
    , nQuality(a2.nQuality)
    , nDecExp(a2.nDecExp)
{
}
