#ifndef _CFGCFGFAMILYWARTABLE_H_
#define _CFGCFGFAMILYWARTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgFamilyWarTable - Config data class
 */
class CfgFamilyWarTable
{
public:
    CfgFamilyWarTable();
    ~CfgFamilyWarTable();

    void AddParam(int32_t nKey, int32_t nValue);

    std::map<int, int> m_mParams;
};

#endif // _CFGCFGFAMILYWARTABLE_H_
