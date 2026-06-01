#ifndef _CFGQQGIFT_H_
#define _CFGQQGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgQQGift - Config data class
 */
class CfgQQGift
{
public:
    int32_t nIndex;
    int32_t nType;
    int32_t nCondition;
    std::vector<MemChrBag> vReward;

    CfgQQGift() = default;
    CfgQQGift(const CfgQQGift& a2);
    ~CfgQQGift() = default;
    CfgQQGift& operator=(const CfgQQGift& a2);
};

#endif // _CFGQQGIFT_H_
