#include "Config/CfgShouChong.h"


CfgShouChong::CfgShouChong(const CfgShouChong& a2)
{
    nIndex = a2.nIndex;
    nGold = a2.nGold;
    nBroadcast = a2.nBroadcast;
    vReward = a2.vReward;
}

CfgShouChong& CfgShouChong::operator=(const CfgShouChong& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nGold = a2.nGold;
        nBroadcast = a2.nBroadcast;
        vReward = a2.vReward;
    }
    return *this;
}
