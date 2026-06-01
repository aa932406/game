#include "Config/CfgCarrier.h"


CfgCarrier::CfgCarrier(const CfgCarrier& a2)
{
    nId = a2.nId;
    lSkills = a2.lSkills;
}

CfgCarrier& CfgCarrier::operator=(const CfgCarrier& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        lSkills = a2.lSkills;
    }
    return *this;
}
