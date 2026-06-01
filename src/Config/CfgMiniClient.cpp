#include "Config/CfgMiniClient.h"


CfgMiniClient::CfgMiniClient(const CfgMiniClient& a2)
{
    nIconLogin = a2.nIconLogin;
    nIconDownload = a2.nIconDownload;
    vReward = a2.vReward;
}

CfgMiniClient& CfgMiniClient::operator=(const CfgMiniClient& a2)
{
    if (this != &a2)
    {
        nIconLogin = a2.nIconLogin;
        nIconDownload = a2.nIconDownload;
        vReward = a2.vReward;
    }
    return *this;
}
