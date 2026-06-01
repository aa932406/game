#ifndef _CFGCFGGOLDEGG_H_
#define _CFGCFGGOLDEGG_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgGoldEgg - Config data class
 */
class CfgGoldEgg
{
public:
    int32_t nId;
    int32_t nGroupId;
    int32_t nOpenTimes;
    int32_t nCostMoney;
    int32_t nCostGold;
    std::vector<MemChrBag> vReward;

    CfgGoldEgg() = default;
    ~CfgGoldEgg() = default;
    CfgGoldEgg(const CfgGoldEgg& a2);
    CfgGoldEgg& operator=(const CfgGoldEgg& a2);
};

#endif // _CFGCFGGOLDEGG_H_
