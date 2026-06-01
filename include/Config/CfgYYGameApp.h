#ifndef _CFGYYGAMEAPP_H_
#define _CFGYYGAMEAPP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgYYGameApp - Config data class
 */
class CfgYYGameApp
{
public:
    int32_t nIndex;
    int32_t nType;
    std::vector<MemChrBag> Rewards;
    int32_t nCondition;

    CfgYYGameApp() = default;
    CfgYYGameApp(const CfgYYGameApp& a2);
    ~CfgYYGameApp() = default;
    CfgYYGameApp& operator=(const CfgYYGameApp& a2);
};

#endif // _CFGYYGAMEAPP_H_
