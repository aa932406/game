#ifndef _CFGCARRIERATTR_H_
#define _CFGCARRIERATTR_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgCarrierAttr - Config data class
 */
class CfgCarrierAttr
{
public:
    int32_t nId;
    int32_t nLevel;
    std::vector<AttrAddon> vAttr;

    CfgCarrierAttr() = default;
    CfgCarrierAttr(const CfgCarrierAttr& a2);
    ~CfgCarrierAttr() = default;
    CfgCarrierAttr& operator=(const CfgCarrierAttr& a2);
};

#endif // _CFGCARRIERATTR_H_
