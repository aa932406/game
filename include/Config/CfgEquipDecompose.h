#ifndef _CFGCFGEQUIPDECOMPOSE_H_
#define _CFGCFGEQUIPDECOMPOSE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgEquipDecompose - Config data class
 */
class CfgEquipDecompose
{
public:
    int32_t m_nClass;
    int32_t m_nId;
    int32_t m_nCostMoney;
    std::vector<MemChrBag> m_vGiveItems;
    int32_t m_nLimitType;
    int32_t m_bDropEquip;
    int32_t m_nRateEquip;

    CfgEquipDecompose() = default;
    ~CfgEquipDecompose() = default;
    CfgEquipDecompose(const CfgEquipDecompose& a2);
    CfgEquipDecompose& operator=(const CfgEquipDecompose& a2);
};

#endif // _CFGCFGEQUIPDECOMPOSE_H_
