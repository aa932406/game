#include "Config/CfgOutLinkFestival.h"


CfgOutLinkFestival::CfgOutLinkFestival(const CfgOutLinkFestival& a2)
{
    nIndex = a2.nIndex;
    nStartTime = a2.nStartTime;
    nEndTime = a2.nEndTime;
    nIcon = a2.nIcon;
    strPlatfrom = a2.strPlatfrom;
}

CfgOutLinkFestival& CfgOutLinkFestival::operator=(const CfgOutLinkFestival& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nStartTime = a2.nStartTime;
        nEndTime = a2.nEndTime;
        nIcon = a2.nIcon;
        strPlatfrom = a2.strPlatfrom;
    }
    return *this;
}
