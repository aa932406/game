#include "Other/LingZhuCfg.h"

LingZhuCfg::LingZhuCfg()
    : nId(0)
    , nType(0)
    , nLevel(0)
    , nKunLingLevel(0)
    , nQuality(0)
    , nDecExp(0)
{
}

LingZhuCfg::LingZhuCfg(const LingZhuCfg& a2)
    : nId(a2.nId)
    , nType(a2.nType)
    , nLevel(a2.nLevel)
    , lAttrList(a2.lAttrList)
    , nKunLingLevel(a2.nKunLingLevel)
    , nQuality(a2.nQuality)
    , nDecExp(a2.nDecExp)
{
}
