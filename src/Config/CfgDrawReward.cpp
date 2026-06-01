#include "Config/CfgDrawReward.h"

CfgDrawReward::CfgDrawReward(const CfgDrawReward& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    nRate = a2.nRate;
    nBroad = a2.nBroad;
    vItem = a2.vItem;
    strGoodsName = a2.strGoodsName;
}

CfgDrawReward& CfgDrawReward::operator=(const CfgDrawReward& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nType = a2.nType;
        nRate = a2.nRate;
        nBroad = a2.nBroad;
        vItem = a2.vItem;
        strGoodsName = a2.strGoodsName;
    }
    return *this;
}
