#include "Config/CfgSevenLoginRewrad.h"


CfgSevenLoginRewrad::CfgSevenLoginRewrad(const CfgSevenLoginRewrad& a2)
{
    Day = a2.Day;
    ItemVector = a2.ItemVector;
}

CfgSevenLoginRewrad& CfgSevenLoginRewrad::operator=(const CfgSevenLoginRewrad& a2)
{
    if (this != &a2)
    {
        Day = a2.Day;
        ItemVector = a2.ItemVector;
    }
    return *this;
}
