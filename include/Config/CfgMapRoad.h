#ifndef _CFGMAPROAD_H_
#define _CFGMAPROAD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgMapRoad - Config data class
 */
class CfgMapRoad
{
public:
    int32_t nIndex;
    int32_t nNextIndex;
    int32_t nMapId;
    std::list<Position> road;

    CfgMapRoad() = default;
    CfgMapRoad(const CfgMapRoad& a2);
    ~CfgMapRoad() = default;
    CfgMapRoad& operator=(const CfgMapRoad& a2);
};

#endif // _CFGMAPROAD_H_
