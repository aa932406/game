#include "Config/CfgCarrierAttr.h"


CfgCarrierAttr::CfgCarrierAttr(const CfgCarrierAttr& a2)
{
    nId = a2.nId;
    nLevel = a2.nLevel;
    vAttr = a2.vAttr;
}

CfgCarrierAttr& CfgCarrierAttr::operator=(const CfgCarrierAttr& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nLevel = a2.nLevel;
        vAttr = a2.vAttr;
    }
    return *this;
}
