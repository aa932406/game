#ifndef _CFGPASSIVESKILL_H_
#define _CFGPASSIVESKILL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgPassiveSkill - Config data class
 */
class CfgPassiveSkill
{
public:
    int32_t id;
    int32_t type;
    std::vector<AttrAddon> vAttrs;
    std::list<TalentAddon> lTalentAddon;
    int32_t dropMoneyRate;

    CfgPassiveSkill() = default;
    CfgPassiveSkill(const CfgPassiveSkill& a2);
    ~CfgPassiveSkill() = default;
    CfgPassiveSkill& operator=(const CfgPassiveSkill& a2);
};

#endif // _CFGPASSIVESKILL_H_
