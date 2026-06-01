#ifndef _CFGSIGNREWARD_H_
#define _CFGSIGNREWARD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgSignReward - Config data class
 */
class CfgSignReward
{
public:
    int32_t count;
    std::vector<MemChrBag> ItemVector;

    CfgSignReward() = default;
    CfgSignReward(const CfgSignReward& a2);
    ~CfgSignReward() = default;
    CfgSignReward& operator=(const CfgSignReward& a2);
};

#endif // _CFGSIGNREWARD_H_
