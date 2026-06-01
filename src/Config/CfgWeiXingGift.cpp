#include "Config/CfgWeiXingGift.h"


CfgWeiXingGift::CfgWeiXingGift(const CfgWeiXingGift& a2)
{
    nIconId = a2.nIconId;
    vReward = a2.vReward;
}

CfgWeiXingGift& CfgWeiXingGift::operator=(const CfgWeiXingGift& a2)
{
    if (this != &a2)
    {
        nIconId = a2.nIconId;
        vReward = a2.vReward;
    }
    return *this;
}
