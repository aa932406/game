#include "Config/CfgMountEquip.h"

CfgMountEquip::CfgMountEquip(const CfgMountEquip& a2)
{
    nId = a2.nId;
    nType = a2.nType;
    nQuality = a2.nQuality;
    vAttr = a2.vAttr;
}

CfgMountEquip& CfgMountEquip::operator=(const CfgMountEquip& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nType = a2.nType;
        nQuality = a2.nQuality;
        vAttr = a2.vAttr;
    }
    return *this;
}
