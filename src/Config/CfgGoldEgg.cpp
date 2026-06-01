#include "Config/CfgGoldEgg.h"

CfgGoldEgg::CfgGoldEgg(const CfgGoldEgg& a2)
{
    nId = a2.nId;
    nGroupId = a2.nGroupId;
    nOpenTimes = a2.nOpenTimes;
    nCostMoney = a2.nCostMoney;
    nCostGold = a2.nCostGold;
    vReward = a2.vReward;
}

CfgGoldEgg& CfgGoldEgg::operator=(const CfgGoldEgg& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nGroupId = a2.nGroupId;
        nOpenTimes = a2.nOpenTimes;
        nCostMoney = a2.nCostMoney;
        nCostGold = a2.nCostGold;
        vReward = a2.vReward;
    }
    return *this;
}
