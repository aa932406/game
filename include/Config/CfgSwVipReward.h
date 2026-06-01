#ifndef _CFGSWVIPREWARD_H_
#define _CFGSWVIPREWARD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgSwVipReward - Config data class
 */
class CfgSwVipReward
{
public:
    int32_t nIndex;
    int32_t nType;
    int32_t nCondition;
    std::vector<MemChrBag> vReward;

    CfgSwVipReward() = default;
    CfgSwVipReward(const CfgSwVipReward& a2);
    ~CfgSwVipReward() = default;
    CfgSwVipReward& operator=(const CfgSwVipReward& a2);
};

#endif // _CFGSWVIPREWARD_H_
