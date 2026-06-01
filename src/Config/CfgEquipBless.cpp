#include "Config/CfgEquipBless.h"

CfgEquipBless::CfgEquipBless(const CfgEquipBless& a2)
{
    nId = a2.nId;
    nType = a2.nType;
    nBlessLevel = a2.nBlessLevel;
    nCostMoney = a2.nCostMoney;
    nRate = a2.nRate;
    vAttr = a2.vAttr;
}

CfgEquipBless& CfgEquipBless::operator=(const CfgEquipBless& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nType = a2.nType;
        nBlessLevel = a2.nBlessLevel;
        nCostMoney = a2.nCostMoney;
        nRate = a2.nRate;
        vAttr = a2.vAttr;
    }
    return *this;
}
