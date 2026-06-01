#include "Config/CfgYYVip.h"


CfgYYVip::CfgYYVip(const CfgYYVip& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    Rewards = a2.Rewards;
    nCondition = a2.nCondition;
    nPric = a2.nPric;
    nLimit = a2.nLimit;
}

CfgYYVip& CfgYYVip::operator=(const CfgYYVip& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nType = a2.nType;
        Rewards = a2.Rewards;
        nCondition = a2.nCondition;
        nPric = a2.nPric;
        nLimit = a2.nLimit;
    }
    return *this;
}
