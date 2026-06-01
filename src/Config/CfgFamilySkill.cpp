#include "Config/CfgFamilySkill.h"


CfgFamilySkill::CfgFamilySkill(const CfgFamilySkill& a2)
{
    nId = a2.nId;
    nLevel = a2.nLevel;
    nFamilyLevel = a2.nFamilyLevel;
    nCostValue = a2.nCostValue;
    vAttrAddon = a2.vAttrAddon;
    nCostMoney = a2.nCostMoney;
    PlayerLevel = a2.PlayerLevel;
}

CfgFamilySkill& CfgFamilySkill::operator=(const CfgFamilySkill& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nLevel = a2.nLevel;
        nFamilyLevel = a2.nFamilyLevel;
        nCostValue = a2.nCostValue;
        vAttrAddon = a2.vAttrAddon;
        nCostMoney = a2.nCostMoney;
        PlayerLevel = a2.PlayerLevel;
    }
    return *this;
}
