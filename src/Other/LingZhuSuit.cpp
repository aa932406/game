#include "Other/LingZhuSuit.h"

LingZhuSuit::LingZhuSuit()
    : nSuitId(0)
    , nCount(0)
{
}

LingZhuSuit::LingZhuSuit(const LingZhuSuit& a2)
    : nSuitId(a2.nSuitId)
    , lAttrList(a2.lAttrList)
    , nCount(a2.nCount)
{
}
