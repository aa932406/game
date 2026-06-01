#include "Config/CfgShiZhuangLevel.h"


CfgShiZhuangLevel::CfgShiZhuangLevel(const CfgShiZhuangLevel& a2)
{
    nType = a2.nType;
    nLevel = a2.nLevel;
    nLevelExp = a2.nLevelExp;
    nNeedLevel = a2.nNeedLevel;
    nCostItem = a2.nCostItem;
    nGetExp = a2.nGetExp;
    nSmallCritRate = a2.nSmallCritRate;
    nLargeCritRate = a2.nLargeCritRate;
    nBroadcast = a2.nBroadcast;
    vAttr = a2.vAttr;
}

CfgShiZhuangLevel& CfgShiZhuangLevel::operator=(const CfgShiZhuangLevel& a2)
{
    if (this != &a2)
    {
        nType = a2.nType;
        nLevel = a2.nLevel;
        nLevelExp = a2.nLevelExp;
        nNeedLevel = a2.nNeedLevel;
        nCostItem = a2.nCostItem;
        nGetExp = a2.nGetExp;
        nSmallCritRate = a2.nSmallCritRate;
        nLargeCritRate = a2.nLargeCritRate;
        nBroadcast = a2.nBroadcast;
        vAttr = a2.vAttr;
    }
    return *this;
}
