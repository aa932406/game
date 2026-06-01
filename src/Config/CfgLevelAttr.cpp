#include "Config/CfgLevelAttr.h"


CfgLevelAttr::CfgLevelAttr(const CfgLevelAttr& a2)
{
    job = a2.job;
    level = a2.level;
    addonattr = a2.addonattr;
    addonPoint = a2.addonPoint;
}

CfgLevelAttr& CfgLevelAttr::operator=(const CfgLevelAttr& a2)
{
    if (this != &a2)
    {
        job = a2.job;
        level = a2.level;
        addonattr = a2.addonattr;
        addonPoint = a2.addonPoint;
    }
    return *this;
}
