#ifndef _CFGCFGEXCHANGE_H_
#define _CFGCFGEXCHANGE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgExchange - Config data class
 */
class CfgExchange
{
public:
    int32_t nType;
    int32_t nIndex;
    int32_t nLimit;
    std::list<ItemData> vCost;
    std::vector<MemChrBag> vReward;

    CfgExchange() = default;
    ~CfgExchange() = default;
    CfgExchange(const CfgExchange& a2);
    CfgExchange& operator=(const CfgExchange& a2);
};

#endif // _CFGCFGEXCHANGE_H_
