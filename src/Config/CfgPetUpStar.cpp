#include "Config/CfgPetUpStar.h"


CfgPetUpStar::CfgPetUpStar(const CfgPetUpStar& a2)
{
    nStar = a2.nStar;
    nCostMoney = a2.nCostMoney;
    lCostItem = a2.lCostItem;
    vAddPetAttr = a2.vAddPetAttr;
    vAddPlayerAttr = a2.vAddPlayerAttr;
    vSkillLevel = a2.vSkillLevel;
    GongGaoId = a2.GongGaoId;
    nCostGold = a2.nCostGold;
}

CfgPetUpStar& CfgPetUpStar::operator=(const CfgPetUpStar& a2)
{
    if (this != &a2)
    {
        nStar = a2.nStar;
        nCostMoney = a2.nCostMoney;
        lCostItem = a2.lCostItem;
        vAddPetAttr = a2.vAddPetAttr;
        vAddPlayerAttr = a2.vAddPlayerAttr;
        vSkillLevel = a2.vSkillLevel;
        GongGaoId = a2.GongGaoId;
        nCostGold = a2.nCostGold;
    }
    return *this;
}
