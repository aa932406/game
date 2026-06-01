#include "Config/CfgQQGift.h"


CfgQQGift::CfgQQGift(const CfgQQGift& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    nCondition = a2.nCondition;
    vReward = a2.vReward;
}

CfgQQGift& CfgQQGift::operator=(const CfgQQGift& a2)
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
