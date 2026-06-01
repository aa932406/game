#include "Config/CfgCampWarReward.h"

CfgCampWarReward::CfgCampWarReward(const CfgCampWarReward& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    nCondition = a2.nCondition;
    vReward = a2.vReward;
}

CfgCampWarReward& CfgCampWarReward::operator=(const CfgCampWarReward& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nType = a2.nType;
        nCondition = a2.nCondition;
        vReward = a2.vReward;
    }
    return *this;
}
