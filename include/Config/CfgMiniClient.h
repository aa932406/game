#ifndef _CFGMINICLIENT_H_
#define _CFGMINICLIENT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMiniClient - Config data class
 */
class CfgMiniClient
{
public:
    int32_t nIconLogin;
    int32_t nIconDownload;
    std::vector<MemChrBag> vReward;

    CfgMiniClient() = default;
    CfgMiniClient(const CfgMiniClient& a2);
    ~CfgMiniClient() = default;
    CfgMiniClient& operator=(const CfgMiniClient& a2);
};

#endif // _CFGMINICLIENT_H_
