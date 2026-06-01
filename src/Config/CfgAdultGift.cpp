#include "Config/CfgAdultGift.h"


CfgAdultGift::CfgAdultGift(const CfgAdultGift& a2)
{
    nIconId = a2.nIconId;
    vReward = a2.vReward;
}

CfgAdultGift& CfgAdultGift::operator=(const CfgAdultGift& a2)
{
    if (this != &a2)
    {
        nIconId = a2.nIconId;
        vReward = a2.vReward;
    }
    return *this;
}
