#ifndef _CFGMAINTAINCOMPENSATE_H_
#define _CFGMAINTAINCOMPENSATE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMaintainCompensate - Config data class
 */
class CfgMaintainCompensate
{
public:
    int32_t nIndex;
    int32_t nTime;
    std::vector<MemChrBag> vItems;

    CfgMaintainCompensate() = default;
    CfgMaintainCompensate(const CfgMaintainCompensate& a2);
    ~CfgMaintainCompensate() = default;
    CfgMaintainCompensate& operator=(const CfgMaintainCompensate& a2);
};

#endif // _CFGMAINTAINCOMPENSATE_H_
