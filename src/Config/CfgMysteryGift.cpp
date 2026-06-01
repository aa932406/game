#include "Config/CfgMysteryGift.h"


CfgMysteryGift::CfgMysteryGift(const CfgMysteryGift& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    nCondition = a2.nCondition;
    nBroadId = a2.nBroadId;
    vItem = a2.vItem;
}

CfgMysteryGift& CfgMysteryGift::operator=(const CfgMysteryGift& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nType = a2.nType;
        nCondition = a2.nCondition;
        nBroadId = a2.nBroadId;
        vItem = a2.vItem;
    }
    return *this;
}
