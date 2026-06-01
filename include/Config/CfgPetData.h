#ifndef _CFGPETDATA_H_
#define _CFGPETDATA_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgPetData - Config data class
 */
class CfgPetData
{
public:
    int32_t m_nPetId;
    int32_t m_Quality;
    std::vector<AttrAddon> m_vBaseAttr;

    CfgPetData() = default;
    CfgPetData(const CfgPetData& a2);
    ~CfgPetData() = default;
    CfgPetData& operator=(const CfgPetData& a2);
};

#endif // _CFGPETDATA_H_
