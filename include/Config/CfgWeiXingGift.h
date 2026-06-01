#ifndef _CFGWEIXINGGIFT_H_
#define _CFGWEIXINGGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgWeiXingGift - Config data class
 */
class CfgWeiXingGift
{
public:
    int32_t nIconId;
    std::vector<MemChrBag> vReward;

    CfgWeiXingGift() = default;
    CfgWeiXingGift(const CfgWeiXingGift& a2);
    ~CfgWeiXingGift() = default;
    CfgWeiXingGift& operator=(const CfgWeiXingGift& a2);
};

#endif // _CFGWEIXINGGIFT_H_
