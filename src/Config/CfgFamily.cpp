#include "Config/CfgFamily.h"

CfgFamily::CfgFamily(const CfgFamily& a2)
{
    nLevel = a2.nLevel;
    nExp = a2.nExp;
    nMaxMembers = a2.nMaxMembers;
    nMaxBossPoints = a2.nMaxBossPoints;
    vPosition_0 = a2.vPosition_0;
    vPosition_1 = a2.vPosition_1;
    vPosition_2 = a2.vPosition_2;
    vPosition_3 = a2.vPosition_3;
}

CfgFamily& CfgFamily::operator=(const CfgFamily& a2)
{
    if (this != &a2)
    {
        nLevel = a2.nLevel;
        nExp = a2.nExp;
        nMaxMembers = a2.nMaxMembers;
        nMaxBossPoints = a2.nMaxBossPoints;
        vPosition_0 = a2.vPosition_0;
        vPosition_1 = a2.vPosition_1;
        vPosition_2 = a2.vPosition_2;
        vPosition_3 = a2.vPosition_3;
    }
    return *this;
}
