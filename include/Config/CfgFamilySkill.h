#ifndef _CFGFAMILYSKILL_H_
#define _CFGFAMILYSKILL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgFamilySkill - Config data class
 */
class CfgFamilySkill
{
public:
    int32_t nId;
    int32_t nLevel;
    int32_t nFamilyLevel;
    int32_t nCostValue;
    std::vector<AttrAddon> vAttrAddon;
    int32_t nCostMoney;
    int32_t PlayerLevel;

    CfgFamilySkill() = default;
    CfgFamilySkill(const CfgFamilySkill& a2);
    ~CfgFamilySkill() = default;
    CfgFamilySkill& operator=(const CfgFamilySkill& a2);
};

#endif // _CFGFAMILYSKILL_H_
