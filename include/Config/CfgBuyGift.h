#ifndef _CFGBUYGIFT_H_
#define _CFGBUYGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgBuyGift - Config data class
 */
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

#endif // _CFGBUYGIFT_H_
