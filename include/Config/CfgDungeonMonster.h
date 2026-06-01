#ifndef _CFGDUNGEONMONSTER_H_
#define _CFGDUNGEONMONSTER_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgDungeonMonster - Config data class
 */
class CfgDungeonMonster
{
public:
    int32_t id;
    int32_t wave;
    std::list<Param2> mids;
    int32_t x;
    int32_t y;
    int32_t count;
    int32_t side;
    std::list<Position> road;
    int32_t delay;
    int32_t times;
    int32_t money;
    int32_t life;
    std::vector<Position> randpos;
    int32_t wait;

    CfgDungeonMonster() = default;
    CfgDungeonMonster(const CfgDungeonMonster& a2);
    ~CfgDungeonMonster() = default;
    CfgDungeonMonster& operator=(const CfgDungeonMonster& a2);
};

#endif // _CFGDUNGEONMONSTER_H_
