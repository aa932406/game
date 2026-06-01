#include "Config/CfgOnlineReward.h"


CfgOnlineReward::CfgOnlineReward(const CfgOnlineReward& a2)
{
    Id = a2.Id;
    NeedTime = a2.NeedTime;
    ItemVector = a2.ItemVector;
}

CfgOnlineReward& CfgOnlineReward::operator=(const CfgOnlineReward& a2)
{
    if (this != &a2)
    {
        Id = a2.Id;
        NeedTime = a2.NeedTime;
        ItemVector = a2.ItemVector;
    }
    return *this;
}
