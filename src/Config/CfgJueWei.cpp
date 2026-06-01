#include "Config/CfgJueWei.h"


CfgJueWei::CfgJueWei(const CfgJueWei& a2)
{
    nId = a2.nId;
    nNeedLevel = a2.nNeedLevel;
    nNeedViGour = a2.nNeedViGour;
    vAttr = a2.vAttr;
}

CfgJueWei& CfgJueWei::operator=(const CfgJueWei& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nNeedLevel = a2.nNeedLevel;
        nNeedViGour = a2.nNeedViGour;
        vAttr = a2.vAttr;
    }
    return *this;
}
