#ifndef _CFGEQUIPSUIT_H_
#define _CFGEQUIPSUIT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgEquipSuit - Config data class
 */
class CfgEquipSuit
{
public:
    int32_t m_nId;
    int32_t m_nCount;
    std::vector<AttrAddon> m_lstSuitAttr;
    std::list<int> m_IdList;
    int32_t m_nBuffId;

    CfgEquipSuit() = default;
    CfgEquipSuit(const CfgEquipSuit& a2);
    ~CfgEquipSuit() = default;
    CfgEquipSuit& operator=(const CfgEquipSuit& a2);
};

#endif // _CFGEQUIPSUIT_H_
