#include "Other/LittleHelperCfg.h"

LittleHelperCfg::LittleHelperCfg()
    : nId(0)
    , nSpeed(0)
    , nGold(0)
    , nReGold(0)
    , nTime(0)
    , nGongGaoId(0)
{
}

LittleHelperCfg::LittleHelperCfg(const LittleHelperCfg& a2)
    : nId(a2.nId)
    , nSpeed(a2.nSpeed)
    , lAttr(a2.lAttr)
    , nGold(a2.nGold)
    , nReGold(a2.nReGold)
    , nTime(a2.nTime)
    , nGongGaoId(a2.nGongGaoId)
{
}
