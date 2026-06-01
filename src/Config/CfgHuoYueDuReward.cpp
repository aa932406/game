#include "Config/CfgHuoYueDuReward.h"


CfgHuoYueDuReward::CfgHuoYueDuReward(const CfgHuoYueDuReward& a2)
{
    Id = a2.Id;
    NeedHuoYueDu = a2.NeedHuoYueDu;
    Items = a2.Items;
}

CfgHuoYueDuReward& CfgHuoYueDuReward::operator=(const CfgHuoYueDuReward& a2)
{
    if (this != &a2)
    {
        Id = a2.Id;
        NeedHuoYueDu = a2.NeedHuoYueDu;
        Items = a2.Items;
    }
    return *this;
}
