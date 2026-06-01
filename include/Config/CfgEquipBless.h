#ifndef _CFGCFGEQUIPBLESS_H_
#define _CFGCFGEQUIPBLESS_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgEquipBless - Config data class
 */
class CfgEquipBless
{
public:
    int32_t nId;
    int32_t nType;
    int32_t nBlessLevel;
    int32_t nCostMoney;
    int32_t nRate;
    std::vector<AttrAddon> vAttr;

    CfgEquipBless() = default;
    ~CfgEquipBless() = default;
    CfgEquipBless(const CfgEquipBless& a2);
    CfgEquipBless& operator=(const CfgEquipBless& a2);
};

#endif // _CFGCFGEQUIPBLESS_H_
