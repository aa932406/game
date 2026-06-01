#include "Config/CfgTencentGift.h"


CfgTencentGift::CfgTencentGift(const CfgTencentGift& a2)
{
    nLevel = a2.nLevel;
    vRewards = a2.vRewards;
    vVipRewards = a2.vVipRewards;
}

CfgTencentGift& CfgTencentGift::operator=(const CfgTencentGift& a2)
{
    if (this != &a2)
    {
        nLevel = a2.nLevel;
        vRewards = a2.vRewards;
        vVipRewards = a2.vVipRewards;
    }
    return *this;
}
