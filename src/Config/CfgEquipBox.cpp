#include "Config/CfgEquipBox.h"

CfgEquipBox::CfgEquipBox(const CfgEquipBox& a2)
{
    nId = a2.nId;
    nWeight = a2.nWeight;
    nBind = a2.nBind;
    lstStar = a2.lstStar;
    nProbility = a2.nProbility;
}

CfgEquipBox& CfgEquipBox::operator=(const CfgEquipBox& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nWeight = a2.nWeight;
        nBind = a2.nBind;
        lstStar = a2.lstStar;
        nProbility = a2.nProbility;
    }
    return *this;
}
