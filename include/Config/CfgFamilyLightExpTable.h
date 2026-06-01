#ifndef _CFGCFGFAMILYLIGHTEXPTABLE_H_
#define _CFGCFGFAMILYLIGHTEXPTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgFamilyLightExpTable - Config data class
 */
class CfgFamilyLightExpTable
{
public:
    CfgFamilyLightExpTable();
    ~CfgFamilyLightExpTable();

    void Add(int32_t nLevel, int32_t nExp);

    std::map<int, int> m_mLightExp;
};

#endif // _CFGCFGFAMILYLIGHTEXPTABLE_H_
