#ifndef _CFGTENCENTSEVENDAYLOGIN_H_
#define _CFGTENCENTSEVENDAYLOGIN_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgTencentSevenDayLogin - Config data class
 */
class CfgTencentSevenDayLogin
{
public:
    int32_t nDays;
    std::vector<MemChrBag> vReward;

    CfgTencentSevenDayLogin() = default;
    CfgTencentSevenDayLogin(const CfgTencentSevenDayLogin& a2);
    ~CfgTencentSevenDayLogin() = default;
    CfgTencentSevenDayLogin& operator=(const CfgTencentSevenDayLogin& a2);
};

#endif // _CFGTENCENTSEVENDAYLOGIN_H_
