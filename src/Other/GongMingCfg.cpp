#include "Other/GongMingCfg.h"

GongMingCfg::GongMingCfg()
    : nChong(0)
    , nStar(0)
    , nIndex(0)
    , NeedGongMingZhi(0)
    , GongGaoId(0)
{
}

GongMingCfg::GongMingCfg(const GongMingCfg& a2)
    : nChong(a2.nChong)
    , nStar(a2.nStar)
    , nIndex(a2.nIndex)
    , NeedGongMingZhi(a2.NeedGongMingZhi)
    , lAttrList(a2.lAttrList)
    , GongGaoId(a2.GongGaoId)
{
}
