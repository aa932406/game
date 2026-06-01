#include "Config/CfgYYGameApp.h"


CfgYYGameApp::CfgYYGameApp(const CfgYYGameApp& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    Rewards = a2.Rewards;
    nCondition = a2.nCondition;
}

CfgYYGameApp& CfgYYGameApp::operator=(const CfgYYGameApp& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nType = a2.nType;
        Rewards = a2.Rewards;
        nCondition = a2.nCondition;
    }
    return *this;
}
