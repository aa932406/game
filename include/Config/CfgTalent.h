#ifndef _CFGTALENT_H_
#define _CFGTALENT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgTalent - Config data class
 */
class CfgTalent
{
public:
    int32_t id;
    int32_t Level;
    int32_t Playerlevel;
    int32_t maxLevel;
    int32_t skillid;
    std::list<ItemData> costItem;
    std::list<int> powerSkills;
    int32_t battle;
    int32_t Point;
    int32_t GongGaoId;

    CfgTalent() = default;
    CfgTalent(const CfgTalent& a2);
    ~CfgTalent() = default;
    CfgTalent& operator=(const CfgTalent& a2);
};

#endif // _CFGTALENT_H_
