#include "Config/CfgLevelGift.h"


CfgLevelGift::CfgLevelGift(const CfgLevelGift& a2)
{
    Index = a2.Index;
    Level = a2.Level;
    ItemVector = a2.ItemVector;
}

CfgLevelGift& CfgLevelGift::operator=(const CfgLevelGift& a2)
{
    if (this != &a2)
    {
        Index = a2.Index;
        Level = a2.Level;
        ItemVector = a2.ItemVector;
    }
    return *this;
}
