#ifndef _CFGHUOYUEDUREWARD_H_
#define _CFGHUOYUEDUREWARD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgHuoYueDuReward - Config data class
 */
class CfgHuoYueDuReward
{
public:
    int32_t Id;
    int32_t NeedHuoYueDu;
    std::vector<MemChrBag> Items;

    CfgHuoYueDuReward() = default;
    CfgHuoYueDuReward(const CfgHuoYueDuReward& a2);
    ~CfgHuoYueDuReward() = default;
    CfgHuoYueDuReward& operator=(const CfgHuoYueDuReward& a2);
};

#endif // _CFGHUOYUEDUREWARD_H_
