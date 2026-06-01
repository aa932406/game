#include "Config/CfgTGPGift.h"

CfgTGPGift::CfgTGPGift(const CfgTGPGift& a2)
{
    strPF = a2.strPF;
    nIndex = a2.nIndex;
    nLevel = a2.nLevel;
    vReward = a2.vReward;
}

CfgTGPGift& CfgTGPGift::operator=(const CfgTGPGift& a2)
{
    if (this != &a2)
    {
        strPF = a2.strPF;
        nIndex = a2.nIndex;
        nLevel = a2.nLevel;
        vReward = a2.vReward;
    }
    return *this;
}
