#ifndef _CFGCFGBLACKETMARKETTABLE_H_
#define _CFGCFGBLACKETMARKETTABLE_H_

// TODO: Member types estimated, verify against IDA pseudocode

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgBlacketMarketTable - Config data class
 */
class CfgBlacketMarketTable
{
public:
    CfgBlacketMarketTable() = default;
    ~CfgBlacketMarketTable() = default;

    void Add(int32_t nId, int32_t nItemId);

    std::map<int, int> m_mItems;
};

#endif // _CFGCFGBLACKETMARKETTABLE_H_
