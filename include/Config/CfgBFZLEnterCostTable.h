#ifndef _CFGCFGBFZLENTERCOSTTABLE_H_
#define _CFGCFGBFZLENTERCOSTTABLE_H_

// TODO: Member types estimated, verify against IDA pseudocode

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgBFZLEnterCostTable - Config data class
 */
class CfgBFZLEnterCostTable
{
public:
    CfgBFZLEnterCostTable() = default;
    ~CfgBFZLEnterCostTable() = default;

    void Add(int32_t nId, int32_t nCost);

    std::map<int, int> m_mCost;
};

#endif // _CFGCFGBFZLENTERCOSTTABLE_H_
