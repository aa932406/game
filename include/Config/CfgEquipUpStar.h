#ifndef _CFGEQUIPUPSTAR_H_
#define _CFGEQUIPUPSTAR_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgEquipUpStar - Config data class
 */
class CfgEquipUpStar
{
public:
    int32_t m_nType;
    int32_t m_nStar;
    int32_t m_nRate;
    int32_t m_nSuccessAddStar;
    int32_t m_nFailLostStar;
    std::list<ItemData> m_lCosItem;
    int32_t m_nCostMoney;
    std::vector<AttrAddon> m_vAttrAddon;
    int32_t m_nCostXingMai;
    int32_t m_RongLianAttr;

    CfgEquipUpStar() = default;
    CfgEquipUpStar(const CfgEquipUpStar& a2);
    ~CfgEquipUpStar() = default;
    CfgEquipUpStar& operator=(const CfgEquipUpStar& a2);
};

#endif // _CFGEQUIPUPSTAR_H_
