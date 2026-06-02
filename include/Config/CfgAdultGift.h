#ifndef _CFGADULTGIFT_H_
#define _CFGADULTGIFT_H_

#include "Common/CommonTypes.h"
#ifndef MEMCHRBAG_DEFINED
#define MEMCHRBAG_DEFINED
struct MemChrBag {
    int32_t itemId;
    int8_t itemClass;
    int32_t itemCount;
    int8_t bind;
    int32_t endTime;
    int64_t srcId;
};
#endif
#include <cstdint>
#include <vector>

#ifndef CFG_ADULT_GIFT_DEFINED
#define CFG_ADULT_GIFT_DEFINED

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

#endif // CFG_ADULT_GIFT_DEFINED
#endif // _CFGADULTGIFT_H_
