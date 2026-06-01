#include "Config/CfgBuyGift.h"


CfgBuyGift::CfgBuyGift(const CfgBuyGift& a2)
{
    nIndex = a2.nIndex;
    nGold = a2.nGold;
    nBroad = a2.nBroad;
    vGift = a2.vGift;
}

CfgBuyGift& CfgBuyGift::operator=(const CfgBuyGift& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nGold = a2.nGold;
        nBroad = a2.nBroad;
        vGift = a2.vGift;
    }
    return *this;
}
