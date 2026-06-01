#ifndef _CFGCFGWINGTABLE_H_
#define _CFGCFGWINGTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgWingTable - Config data class
 */
class CfgWingTable
{
public:
    CfgWingTable();
    ~CfgWingTable();

    void Add(int32_t nId, int32_t nAttrId);

    int32_t GetAttrId(int32_t nId);
    bool IsExist(int32_t nId);
    void Clear();

    std::map<int, int> m_mWingData;
};

#endif // _CFGCFGWINGTABLE_H_
