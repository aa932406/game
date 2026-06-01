#include "Config/CfgSwVipReward.h"


CfgSwVipReward::CfgSwVipReward(const CfgSwVipReward& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    nCondition = a2.nCondition;
    vReward = a2.vReward;
}

CfgSwVipReward& CfgSwVipReward::operator=(const CfgSwVipReward& a2)
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
