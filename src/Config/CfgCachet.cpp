#include "Config/CfgCachet.h"


CfgCachet::CfgCachet(const CfgCachet& a2)
{
    nLevel = a2.nLevel;
    nNeedCacht = a2.nNeedCacht;
    nAttrList = a2.nAttrList;
    nDeduct = a2.nDeduct;
    nGongGaoId = a2.nGongGaoId;
}

CfgCachet& CfgCachet::operator=(const CfgCachet& a2)
{
    if (this != &a2)
    {
        nLevel = a2.nLevel;
        nNeedCacht = a2.nNeedCacht;
        nAttrList = a2.nAttrList;
        nDeduct = a2.nDeduct;
        nGongGaoId = a2.nGongGaoId;
    }
    return *this;
}
