#include "Config/CfgTalentActive.h"


CfgTalentActive::CfgTalentActive(const CfgTalentActive& a2)
{
    nId = a2.nId;
    lItems = a2.lItems;
}

CfgTalentActive& CfgTalentActive::operator=(const CfgTalentActive& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        lItems = a2.lItems;
    }
    return *this;
}
