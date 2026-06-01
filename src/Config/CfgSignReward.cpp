#include "Config/CfgSignReward.h"


CfgSignReward::CfgSignReward(const CfgSignReward& a2)
{
    count = a2.count;
    ItemVector = a2.ItemVector;
}

CfgSignReward& CfgSignReward::operator=(const CfgSignReward& a2)
{
    if (this != &a2)
    {
        count = a2.count;
        ItemVector = a2.ItemVector;
    }
    return *this;
}
