#ifndef _CFGITEMGEM_H_
#define _CFGITEMGEM_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgItemGem - Config data class
 */
class CfgItemGem
{
public:
    int32_t m_nId;
    int32_t m_nType;
    int32_t m_nLevel;
    int32_t m_nPrice;
    int32_t m_nQuality;
    int32_t m_nLayNum;
    std::list<signed char> m_lAddPos;
    std::vector<AttrAddon> m_vAttrAddon;

    CfgItemGem() = default;
    CfgItemGem(const CfgItemGem& a2);
    ~CfgItemGem() = default;
    CfgItemGem& operator=(const CfgItemGem& a2);
};

#endif // _CFGITEMGEM_H_
