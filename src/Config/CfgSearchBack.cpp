#include "Config/CfgSearchBack.h"


CfgSearchBack::CfgSearchBack(const CfgSearchBack& a2)
{
    nId = a2.nId;
    nType = a2.nType;
    nSubType = a2.nSubType;
    nOpenType = a2.nOpenType;
    nMinLevel = a2.nMinLevel;
    nMaxLevel = a2.nMaxLevel;
    nTimes = a2.nTimes;
    nGoldCost = a2.nGoldCost;
    vItemReward = a2.vItemReward;
    vFreeReward = a2.vFreeReward;
}

CfgSearchBack& CfgSearchBack::operator=(const CfgSearchBack& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        nType = a2.nType;
        nSubType = a2.nSubType;
        nOpenType = a2.nOpenType;
        nMinLevel = a2.nMinLevel;
        nMaxLevel = a2.nMaxLevel;
        nTimes = a2.nTimes;
        nGoldCost = a2.nGoldCost;
        vItemReward = a2.vItemReward;
        vFreeReward = a2.vFreeReward;
    }
    return *this;
}
