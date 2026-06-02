#ifndef _CFGCFGCAMPWARREWARD_H_
#define _CFGCFGCAMPWARREWARD_H_

#include <cstdint>
#include <vector>
#include "Common/CommonTypes.h"


#ifndef MEM_CHR_BAG_DEFINED_HERE
#define MEM_CHR_BAG_DEFINED_HERE
struct MemChrBag {
    int32_t itemId;
    int8_t itemClass;
    int32_t itemCount;
    int8_t bind;
    int32_t endTime;
    int64_t srcId;
};
#endif


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
