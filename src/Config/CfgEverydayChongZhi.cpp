#include "Config/CfgEverydayChongZhi.h"

CfgEverydayChongZhi::CfgEverydayChongZhi(const CfgEverydayChongZhi& a2)
{
    Index = a2.Index;
    Type = a2.Type;
    NeedGold = a2.NeedGold;
    ItemVector = a2.ItemVector;
    GongGaoInfo = a2.GongGaoInfo;
}

CfgEverydayChongZhi& CfgEverydayChongZhi::operator=(const CfgEverydayChongZhi& a2)
{
    if (this != &a2)
    {
        Index = a2.Index;
        Type = a2.Type;
        NeedGold = a2.NeedGold;
        ItemVector = a2.ItemVector;
        GongGaoInfo = a2.GongGaoInfo;
    }
    return *this;
}
