#ifndef _CFGYYVIP_H_
#define _CFGYYVIP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgYYVip - Config data class
 */
class CfgYYVip
{
public:
    int32_t nIndex;
    int32_t nType;
    std::vector<MemChrBag> Rewards;
    int32_t nCondition;
    int32_t nPric;
    int32_t nLimit;

    CfgYYVip() = default;
    CfgYYVip(const CfgYYVip& a2);
    ~CfgYYVip() = default;
    CfgYYVip& operator=(const CfgYYVip& a2);
};

#endif // _CFGYYVIP_H_
