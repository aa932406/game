#ifndef _CFGCFGGEMSUIT_H_
#define _CFGCFGGEMSUIT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgGemSuit - Config data class
 */
class CfgGemSuit
{
public:
    int32_t m_nLevel;
    std::vector<AttrAddon> m_vAttrAddon;

    CfgGemSuit() = default;
    ~CfgGemSuit() = default;
    CfgGemSuit(const CfgGemSuit& a2);
    CfgGemSuit& operator=(const CfgGemSuit& a2);
};

#endif // _CFGCFGGEMSUIT_H_
