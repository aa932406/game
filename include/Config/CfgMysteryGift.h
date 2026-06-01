#ifndef _CFGMYSTERYGIFT_H_
#define _CFGMYSTERYGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMysteryGift - Config data class
 */
class CfgMysteryGift
{
public:
    int32_t nIndex;
    int32_t nType;
    int32_t nCondition;
    int32_t nBroadId;
    std::vector<MemChrBag> vItem;

    CfgMysteryGift() = default;
    CfgMysteryGift(const CfgMysteryGift& a2);
    ~CfgMysteryGift() = default;
    CfgMysteryGift& operator=(const CfgMysteryGift& a2);
};

#endif // _CFGMYSTERYGIFT_H_
