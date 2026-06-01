#include "Config/CfgGroupIcon.h"


CfgGroupIcon::CfgGroupIcon(const CfgGroupIcon& a2)
{
    nId = a2.nId;
    bShowInCross = a2.bShowInCross;
    platfrom = a2.platfrom;
}

CfgGroupIcon& CfgGroupIcon::operator=(const CfgGroupIcon& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        bShowInCross = a2.bShowInCross;
        platfrom = a2.platfrom;
    }
    return *this;
}
