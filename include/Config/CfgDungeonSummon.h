#ifndef _CFGDUNGEONSUMMON_H_
#define _CFGDUNGEONSUMMON_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgDungeonSummon - Config data class
 */
class CfgDungeonSummon
{
public:
    int32_t nDungeon;
    int32_t nIndex;
    int32_t nGold;
    int32_t nLimit;
    std::list<int> lMonsters;
    int32_t ConstItem;
    int32_t FreeCount;

    CfgDungeonSummon() = default;
    CfgDungeonSummon(const CfgDungeonSummon& a2);
    ~CfgDungeonSummon() = default;
    CfgDungeonSummon& operator=(const CfgDungeonSummon& a2);
};

#endif // _CFGDUNGEONSUMMON_H_
