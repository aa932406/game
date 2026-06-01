#include "Config/CfgSouGouSkin.h"


CfgSouGouSkin::CfgSouGouSkin(const CfgSouGouSkin& a2)
{
    nIcon = a2.nIcon;
    vReward = a2.vReward;
}

CfgSouGouSkin& CfgSouGouSkin::operator=(const CfgSouGouSkin& a2)
{
    if (this != &a2)
    {
        nIcon = a2.nIcon;
        vReward = a2.vReward;
    }
    return *this;
}
