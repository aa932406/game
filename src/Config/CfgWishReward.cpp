#include "Config/CfgWishReward.h"

CfgWishReward::CfgWishReward(const CfgWishReward& a2)
{
    nTime = a2.nTime;
    vReward = a2.vReward;
}

CfgWishReward& CfgWishReward::operator=(const CfgWishReward& a2)
{
    if (this != &a2)
    {
        nTime = a2.nTime;
        vReward = a2.vReward;
    }
    return *this;
}
