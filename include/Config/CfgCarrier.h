#ifndef _CFGCARRIER_H_
#define _CFGCARRIER_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgCarrier - Config data class
 */
class CfgCarrier
{
public:
    int32_t nId;
    std::list<int> lSkills;

    CfgCarrier() = default;
    CfgCarrier(const CfgCarrier& a2);
    ~CfgCarrier() = default;
    CfgCarrier& operator=(const CfgCarrier& a2);
};

#endif // _CFGCARRIER_H_
