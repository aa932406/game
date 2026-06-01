#ifndef _CFGPETEQUIP_H_
#define _CFGPETEQUIP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgPetEquip - Config data class
 */
class CfgPetEquip
{
public:
    int32_t nId;
    int32_t nType;
    int32_t nPrice;
    int32_t nQuality;
    int32_t nSuitId;
    std::vector<AttrAddon> vAttr;
    int32_t nBroadcast;
    int32_t bCanDrop;
    int32_t nGrade;
    std::vector<AttrAddon> vOwnerAttr;
    int32_t nNeedStar;

    CfgPetEquip() = default;
    CfgPetEquip(const CfgPetEquip& a2);
    ~CfgPetEquip() = default;
    CfgPetEquip& operator=(const CfgPetEquip& a2);
};

#endif // _CFGPETEQUIP_H_
