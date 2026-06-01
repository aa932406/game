#ifndef _CFGCFGHALLOFFAMETABLE_H_
#define _CFGCFGHALLOFFAMETABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgHallOfFameTable - Config data class
 */
class CfgHallOfFameTable
{
public:
    CfgHallOfFameTable();
    ~CfgHallOfFameTable();

    void Add(int32_t nRank, int32_t nRewardId);

    std::map<int, int> m_mRankReward;
};

#endif // _CFGCFGHALLOFFAMETABLE_H_
