#ifndef _CFGADULTGIFT_H_
#define _CFGADULTGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgAdultGift - Config data class
 */
class CfgAdultGift
{
public:
    int32_t nIconId;
    std::vector<MemChrBag> vReward;

    CfgAdultGift() = default;
    CfgAdultGift(const CfgAdultGift& a2);
    ~CfgAdultGift() = default;
    CfgAdultGift& operator=(const CfgAdultGift& a2);
};

#endif // _CFGADULTGIFT_H_
