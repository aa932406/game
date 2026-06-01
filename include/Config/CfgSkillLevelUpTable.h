#ifndef _CFGSKILLLEVELUPTABLE_H_
#define _CFGSKILLLEVELUPTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>
#include <map>


/**
 * @brief CfgSkillLevelUp - Skill level-up config entry
 */
struct CfgSkillLevelUp
{
    int32_t nSkillId;
    int32_t nLevel;
    int32_t nCostSkill;
    int32_t nCostGold;
    std::vector<MemChrBag> vCostItems;
    std::vector<AttrAddon> vAddAttr;
    int32_t nBattle;
};

/**
 * @brief CfgSkillLevelUpTable - Skill level-up config table
 */
class CfgSkillLevelUpTable
{
public:
    CfgSkillLevelUpTable();
    ~CfgSkillLevelUpTable();

    bool Add(const CfgSkillLevelUp& skill);
    bool AddSkillLevelUp(const CfgSkillLevelUp& skill);
    const CfgSkillLevelUp* GetSkillLevelUp(int32_t nSkillId, int32_t nLevel);
    int32_t GetMaxLevel(int32_t nSkillId);
    bool IsSkillExist(int32_t nSkillId);
    void Clear();

private:
    std::map<int32_t, CfgSkillLevelUp> m_mSkillLevelUp;
};

#endif // _CFGSKILLLEVELUPTABLE_H_
