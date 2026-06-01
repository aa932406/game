#ifndef _CFGEQUIPUPPOS_H_
#define _CFGEQUIPUPPOS_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgEquipUpPos - Config data class
 */
class CfgEquipUpPos
{
public:
    int32_t m_nType;
    int32_t m_nLevel;
    int32_t m_nRate;
    int32_t m_nCostMoney;
    int32_t m_nNeedLevel;
    std::vector<AttrAddon> m_vAttrAddon;
    std::list<ItemData> m_nConstItem;
    int32_t m_GongGaoId;
    int32_t m_BaoHuConst;

    CfgEquipUpPos() = default;
    CfgEquipUpPos(const CfgEquipUpPos& a2);
    ~CfgEquipUpPos() = default;
    CfgEquipUpPos& operator=(const CfgEquipUpPos& a2);
};

#endif // _CFGEQUIPUPPOS_H_
