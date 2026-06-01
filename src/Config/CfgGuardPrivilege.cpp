#include "Config/CfgGuardPrivilege.h"


CfgGuardPrivilege::CfgGuardPrivilege(const CfgGuardPrivilege& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    nDaily = a2.nDaily;
    nParam = a2.nParam;
    vReward = a2.vReward;
}

CfgGuardPrivilege& CfgGuardPrivilege::operator=(const CfgGuardPrivilege& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nType = a2.nType;
        nDaily = a2.nDaily;
        nParam = a2.nParam;
        vReward = a2.vReward;
    }
    return *this;
}
