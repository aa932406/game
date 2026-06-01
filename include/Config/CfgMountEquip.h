#ifndef _CFGCFGMOUNTEQUIP_H_
#define _CFGCFGMOUNTEQUIP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMountEquip - Config data class
 */
class CfgMountEquip
{
public:
    int32_t nId;
    int32_t nType;
    int32_t nQuality;
    std::vector<AttrAddon> vAttr;

    CfgMountEquip() = default;
    ~CfgMountEquip() = default;
    CfgMountEquip(const CfgMountEquip& a2);
    CfgMountEquip& operator=(const CfgMountEquip& a2);
};

#endif // _CFGCFGMOUNTEQUIP_H_
