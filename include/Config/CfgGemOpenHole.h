#ifndef _CFGCFGGEMOPENHOLE_H_
#define _CFGCFGGEMOPENHOLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgGemOpenHole - Config data class
 */
class CfgGemOpenHole
{
public:
    int32_t m_nSlot;
    int32_t m_nCostMoney;
    std::list<ItemData> m_lCostItem;

    CfgGemOpenHole() = default;
    ~CfgGemOpenHole() = default;
    CfgGemOpenHole(const CfgGemOpenHole& a2);
    CfgGemOpenHole& operator=(const CfgGemOpenHole& a2);
};

#endif // _CFGCFGGEMOPENHOLE_H_
