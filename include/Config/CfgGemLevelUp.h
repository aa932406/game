#ifndef _CFGGEMLEVELUP_H_
#define _CFGGEMLEVELUP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgGemLevelUp - Config data class
 */
class CfgGemLevelUp
{
public:
    int32_t nId;
    int32_t nNextId;
    int32_t nCostMoney;
    std::list<ItemData> lCostItems;
    int32_t nBroadId;

    CfgGemLevelUp() = default;
    CfgGemLevelUp(const CfgGemLevelUp& a2);
    ~CfgGemLevelUp() = default;
    CfgGemLevelUp& operator=(const CfgGemLevelUp& a2);
};

#endif // _CFGGEMLEVELUP_H_
