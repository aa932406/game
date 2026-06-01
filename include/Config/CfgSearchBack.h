#ifndef _CFGSEARCHBACK_H_
#define _CFGSEARCHBACK_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgSearchBack - Config data class
 */
class CfgSearchBack
{
public:
    int32_t nId;
    int32_t nType;
    int32_t nSubType;
    int32_t nOpenType;
    int32_t nMinLevel;
    int32_t nMaxLevel;
    int32_t nTimes;
    int32_t nGoldCost;
    std::vector<MemChrBag> vItemReward;
    std::vector<MemChrBag> vFreeReward;

    CfgSearchBack() = default;
    CfgSearchBack(const CfgSearchBack& a2);
    ~CfgSearchBack() = default;
    CfgSearchBack& operator=(const CfgSearchBack& a2);
};

#endif // _CFGSEARCHBACK_H_
