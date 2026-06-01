#ifndef _CFGCFGCAMPWARREWARD_H_
#define _CFGCFGCAMPWARREWARD_H_

#include <cstdint>
#include <vector>
#include "Common/CommonTypes.h"


/**
 * @brief CfgCampWarReward - Config data class
 */
class CfgCampWarReward
{
public:
    CfgCampWarReward() = default;
    ~CfgCampWarReward() = default;
    CfgCampWarReward(const CfgCampWarReward& a2);
    CfgCampWarReward& operator=(const CfgCampWarReward& a2);

    int32_t nIndex;
    int32_t nType;
    int32_t nCondition;
    std::vector<MemChrBag> vReward;
};

#endif // _CFGCFGCAMPWARREWARD_H_
