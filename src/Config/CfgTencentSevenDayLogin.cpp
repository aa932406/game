#include "Config/CfgTencentSevenDayLogin.h"


CfgTencentSevenDayLogin::CfgTencentSevenDayLogin(const CfgTencentSevenDayLogin& a2)
{
    nDays = a2.nDays;
    vReward = a2.vReward;
}

CfgTencentSevenDayLogin& CfgTencentSevenDayLogin::operator=(const CfgTencentSevenDayLogin& a2)
{
    if (this != &a2)
    {
        nDays = a2.nDays;
        vReward = a2.vReward;
    }
    return *this;
}
