#ifndef _CFGEQUIP_H_
#define _CFGEQUIP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgEquip - Config data class
 */
class CfgEquip
{
public:
    int32_t m_nId;
    int32_t m_nType;
    int32_t m_nGrade;
    int32_t m_nLevel;
    int32_t m_nJob;
    int32_t m_nQuality;
    int32_t m_nSuitId;
    int32_t m_nSuitId2;
    int32_t m_nPrice;
    std::vector<AttrAddon> m_vAttrAddon;
    std::vector<AttrAddon> m_vElement;
    int32_t m_DropLuck;
    int32_t m_DropRate;
    int32_t m_BackType;
    int32_t m_BackValue;
    int32_t m_nBroadcast;
    int32_t m_backIndex;
    int32_t m_nLimitStar;
    int32_t m_BackGold;
    std::vector<AttrAddon> m_BaseAttr;
    int32_t m_PolishLevel;
    int32_t m_MoFuHuiShou;
    int32_t m_SellDay;
    int32_t m_GongMingLevel;
    int32_t m_WingEquipRefiningLevel;
    int32_t m_XinMoExp;
    int32_t m_XinMoBag;
    int32_t m_CanRongHe;
    int32_t m_TeJieParam;

    CfgEquip() = default;
    CfgEquip(const CfgEquip& a2);
    ~CfgEquip() = default;
    CfgEquip& operator=(const CfgEquip& a2);
};

#endif // _CFGEQUIP_H_
