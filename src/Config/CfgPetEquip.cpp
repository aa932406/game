#include "Config/CfgPetEquip.h"


CfgPetEquip::CfgPetEquip(const CfgPetEquip& a2)
{
    nId = a2.nId;
    nType = a2.nType;
    nPrice = a2.nPrice;
    nQuality = a2.nQuality;
    nSuitId = a2.nSuitId;
    vAttr = a2.vAttr;
    nBroadcast = a2.nBroadcast;
    bCanDrop = a2.bCanDrop;
    nGrade = a2.nGrade;
    vOwnerAttr = a2.vOwnerAttr;
    nNeedStar = a2.nNeedStar;
}

CfgPetEquip& CfgPetEquip::operator=(const CfgPetEquip& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nType = a2.nType;
        nPrice = a2.nPrice;
        nQuality = a2.nQuality;
        nSuitId = a2.nSuitId;
        vAttr = a2.vAttr;
        nBroadcast = a2.nBroadcast;
        bCanDrop = a2.bCanDrop;
        nGrade = a2.nGrade;
        vOwnerAttr = a2.vOwnerAttr;
        nNeedStar = a2.nNeedStar;
    }
    return *this;
}
