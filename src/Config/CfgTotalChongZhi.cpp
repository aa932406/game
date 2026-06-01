#include "Config/CfgTotalChongZhi.h"


CfgTotalChongZhi::CfgTotalChongZhi(const CfgTotalChongZhi& a2)
{
    Index = a2.Index;
    NeedGold = a2.NeedGold;
    ItemVector = a2.ItemVector;
    GongGaoInfo = a2.GongGaoInfo;
    NewServerday = a2.NewServerday;
}

CfgTotalChongZhi& CfgTotalChongZhi::operator=(const CfgTotalChongZhi& a2)
{
    if (this != &a2)
    {
        Index = a2.Index;
        NeedGold = a2.NeedGold;
        ItemVector = a2.ItemVector;
        GongGaoInfo = a2.GongGaoInfo;
        NewServerday = a2.NewServerday;
    }
    return *this;
}
