#include "Config/CfgExchange.h"

CfgExchange::CfgExchange(const CfgExchange& a2)
{
    nType = a2.nType;
    nIndex = a2.nIndex;
    nLimit = a2.nLimit;
    vCost = a2.vCost;
    vReward = a2.vReward;
}

CfgExchange& CfgExchange::operator=(const CfgExchange& a2)
{
    if (this != &a2)
    {
        nType = a2.nType;
        nIndex = a2.nIndex;
        nLimit = a2.nLimit;
        vCost = a2.vCost;
        vReward = a2.vReward;
    }
    return *this;
}
