#ifndef _CFGSGGAMEAPP_H_
#define _CFGSGGAMEAPP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgSgGameApp - Config data class
 */
class CfgSgGameApp
{
public:
    int32_t nIndex;
    int32_t nType;
    int32_t nCondition;
    std::vector<MemChrBag> vRewards;

    CfgSgGameApp() = default;
    CfgSgGameApp(const CfgSgGameApp& a2);
    ~CfgSgGameApp() = default;
    CfgSgGameApp& operator=(const CfgSgGameApp& a2);
};

#endif // _CFGSGGAMEAPP_H_
