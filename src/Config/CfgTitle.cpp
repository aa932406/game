#include "Config/CfgTitle.h"


CfgTitle::CfgTitle(const CfgTitle& a2)
{
    nId = a2.nId;
    nType = a2.nType;
    vParams = a2.vParams;
    sPlatform = a2.sPlatform;
    nJob = a2.nJob;
    nSex = a2.nSex;
    nSpecial = a2.nSpecial;
    nPriority = a2.nPriority;
    vGetAttr = a2.vGetAttr;
    vDressAttr = a2.vDressAttr;
}

CfgTitle& CfgTitle::operator=(const CfgTitle& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nType = a2.nType;
        vParams = a2.vParams;
        sPlatform = a2.sPlatform;
        nJob = a2.nJob;
        nSex = a2.nSex;
        nSpecial = a2.nSpecial;
        nPriority = a2.nPriority;
        vGetAttr = a2.vGetAttr;
        vDressAttr = a2.vDressAttr;
    }
    return *this;
}
