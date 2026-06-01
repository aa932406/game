#include "Config/CfgMaintainCompensate.h"


CfgMaintainCompensate::CfgMaintainCompensate(const CfgMaintainCompensate& a2)
{
    nIndex = a2.nIndex;
    nTime = a2.nTime;
    vItems = a2.vItems;
}

CfgMaintainCompensate& CfgMaintainCompensate::operator=(const CfgMaintainCompensate& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nTime = a2.nTime;
        vItems = a2.vItems;
    }
    return *this;
}
