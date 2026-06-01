#ifndef _CFGTRAP_H_
#define _CFGTRAP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgTrap - Config data class
 */
class CfgTrap
{
public:
    int32_t id;
    int32_t cd;
    int32_t delay;
    int32_t event_type;
    std::string effect;
    int32_t item_cost;
    int32_t life;

    CfgTrap() = default;
    CfgTrap(const CfgTrap& a2);
    ~CfgTrap() = default;
    CfgTrap& operator=(const CfgTrap& a2);
};

#endif // _CFGTRAP_H_
