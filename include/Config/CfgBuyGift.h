#ifndef _CFGBUYGIFT_H_
#define _CFGBUYGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


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


/**
 * @brief CfgBuyGift - Config data class
 */
#ifndef CFG_BUY_GIFT_DEFINED
#define CFG_BUY_GIFT_DEFINED
class CfgBuyGift
{
public:
    int32_t nIndex;
    int32_t nGold;
    int32_t nBroad;
    std::vector<MemChrBag> vGift;

    CfgBuyGift() = default;
    CfgBuyGift(const CfgBuyGift& a2);
    ~CfgBuyGift() = default;
    CfgBuyGift& operator=(const CfgBuyGift& a2);
};

#endif // CFG_BUY_GIFT_DEFINED
#endif // _CFGBUYGIFT_H_
