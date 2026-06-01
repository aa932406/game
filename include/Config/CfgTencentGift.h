#ifndef _CFGTENCENTGIFT_H_
#define _CFGTENCENTGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgTencentGift - Config data class
 */
class CfgTencentGift
{
public:
    int32_t nLevel;
    std::vector<MemChrBag> vRewards;
    std::vector<MemChrBag> vVipRewards;

    CfgTencentGift() = default;
    CfgTencentGift(const CfgTencentGift& a2);
    ~CfgTencentGift() = default;
    CfgTencentGift& operator=(const CfgTencentGift& a2);
};

#endif // _CFGTENCENTGIFT_H_
