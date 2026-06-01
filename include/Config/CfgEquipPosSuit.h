#ifndef _CFGCFGEQUIPPOSSUIT_H_
#define _CFGCFGEQUIPPOSSUIT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgEquipPosSuit - Config data class
 */
class CfgEquipPosSuit
{
public:
    int32_t m_nLevel;
    std::vector<AttrAddon> m_vAttrAddon;

    CfgEquipPosSuit() = default;
    ~CfgEquipPosSuit() = default;
    CfgEquipPosSuit(const CfgEquipPosSuit& a2);
    CfgEquipPosSuit& operator=(const CfgEquipPosSuit& a2);
};

#endif // _CFGCFGEQUIPPOSSUIT_H_
