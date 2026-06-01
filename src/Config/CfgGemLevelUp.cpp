#include "Config/CfgGemLevelUp.h"


CfgGemLevelUp::CfgGemLevelUp(const CfgGemLevelUp& a2)
{
    nId = a2.nId;
    nNextId = a2.nNextId;
    nCostMoney = a2.nCostMoney;
    lCostItems = a2.lCostItems;
    nBroadId = a2.nBroadId;
}

CfgGemLevelUp& CfgGemLevelUp::operator=(const CfgGemLevelUp& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nNextId = a2.nNextId;
        nCostMoney = a2.nCostMoney;
        lCostItems = a2.lCostItems;
        nBroadId = a2.nBroadId;
    }
    return *this;
}
