#include "Config/CfgSgGameApp.h"


CfgSgGameApp::CfgSgGameApp(const CfgSgGameApp& a2)
{
    nIndex = a2.nIndex;
    nType = a2.nType;
    nCondition = a2.nCondition;
    vRewards = a2.vRewards;
}

CfgSgGameApp& CfgSgGameApp::operator=(const CfgSgGameApp& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nType = a2.nType;
        nCondition = a2.nCondition;
        vRewards = a2.vRewards;
    }
    return *this;
}
