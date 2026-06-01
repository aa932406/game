#ifndef _CFGMOBILEPHONEGIFT_H_
#define _CFGMOBILEPHONEGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMobilePhoneGift - Config data class
 */
class CfgMobilePhoneGift
{
public:
    std::vector<MemChrBag> vItem;
    int32_t nIcon;

    CfgMobilePhoneGift() = default;
    CfgMobilePhoneGift(const CfgMobilePhoneGift& a2);
    ~CfgMobilePhoneGift() = default;
    CfgMobilePhoneGift& operator=(const CfgMobilePhoneGift& a2);
};

#endif // _CFGMOBILEPHONEGIFT_H_
