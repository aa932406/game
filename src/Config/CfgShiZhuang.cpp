#include "Config/CfgShiZhuang.h"


CfgShiZhuang::CfgShiZhuang(const CfgShiZhuang& a2)
{
    nId = a2.nId;
    nType = a2.nType;
    vAttr = a2.vAttr;
    nSuitId = a2.nSuitId;
}

CfgShiZhuang& CfgShiZhuang::operator=(const CfgShiZhuang& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nType = a2.nType;
        vAttr = a2.vAttr;
        nSuitId = a2.nSuitId;
    }
    return *this;
}
