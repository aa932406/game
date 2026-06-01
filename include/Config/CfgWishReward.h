#ifndef _CFGCFGWISHREWARD_H_
#define _CFGCFGWISHREWARD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgWishReward - Config data class
 */
class CfgWishReward
{
public:
    int32_t nTime;
    std::vector<MemChrBag> vReward;

    CfgWishReward() = default;
    ~CfgWishReward() = default;
    CfgWishReward(const CfgWishReward& a2);
    CfgWishReward& operator=(const CfgWishReward& a2);
};

#endif // _CFGCFGWISHREWARD_H_
