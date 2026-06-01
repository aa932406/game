#include "Config/CfgTrailer.h"


CfgTrailer::CfgTrailer(const CfgTrailer& a2)
{
    nId = a2.nId;
    nAttrList = a2.nAttrList;
    TrailerQuality = a2.TrailerQuality;
    nTaskId = a2.nTaskId;
    PureDamage = a2.PureDamage;
}

CfgTrailer& CfgTrailer::operator=(const CfgTrailer& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nAttrList = a2.nAttrList;
        TrailerQuality = a2.TrailerQuality;
        nTaskId = a2.nTaskId;
        PureDamage = a2.PureDamage;
    }
    return *this;
}
