#ifndef _CFGCFGCITYWARCONTREWARD_H_
#define _CFGCFGCITYWARCONTREWARD_H_

// TODO: Member types estimated, verify against IDA pseudocode

#include <cstdint>
#include <vector>
#include "Common/CommonTypes.h"


/**
 * @brief CfgCityWarContReward - Config data class
 */
class CfgCityWarContReward
{
public:
    CfgCityWarContReward() = default;
    ~CfgCityWarContReward() = default;

    int32_t nIndex;
    int32_t nType;
    std::vector<MemChrBag> vReward;
};

#endif // _CFGCFGCITYWARCONTREWARD_H_
