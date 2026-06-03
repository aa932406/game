#ifndef _CFGPASSIVESKILL_H_
#define _CFGPASSIVESKILL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>

#ifndef TALENT_ADDON_DEFINED
#define TALENT_ADDON_DEFINED
struct TalentAddon {
    int nSkillId;
    int nSkillLevel;
};
#endif /* TALENT_ADDON_DEFINED */


#ifndef CFGPASSIVESKILL_DEFINED
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
#endif // CFGPASSIVESKILL_DEFINED

#endif // _CFGPASSIVESKILL_H_
