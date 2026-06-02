#ifndef _CFGACTIVITYMONSTER_H_
#define _CFGACTIVITYMONSTER_H_

#include "Common/CommonTypes.h"
#include "Utility/Position.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgActivityMonster - Config data class
 */
class CfgActivityMonster
{
public:
    int32_t id;
    int32_t wave;
    int32_t mid;
    int32_t x;
    int32_t y;
    int32_t count;
    int32_t side;
    std::list<Position> road;
    int32_t delay;
    int32_t times;
    int32_t buff;
    std::vector<Position> randpos;
    int32_t left;

    CfgActivityMonster() = default;
    CfgActivityMonster(const CfgActivityMonster& a2);
    ~CfgActivityMonster() = default;
    CfgActivityMonster& operator=(const CfgActivityMonster& a2);
};

#endif // _CFGACTIVITYMONSTER_H_
