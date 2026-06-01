#ifndef _CFGACTIVITYNPC_H_
#define _CFGACTIVITYNPC_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgActivityNpc - Config data class
 */
class CfgActivityNpc
{
public:
    int32_t id;
    int32_t activity_id;
    int32_t npc_id;
    int32_t count;
    std::vector<int> region_id;

    CfgActivityNpc() = default;
    CfgActivityNpc(const CfgActivityNpc& a2);
    ~CfgActivityNpc() = default;
    CfgActivityNpc& operator=(const CfgActivityNpc& a2);
};

#endif // _CFGACTIVITYNPC_H_
