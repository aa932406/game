#ifndef _CFGCFGSKILLTABLE_H_
#define _CFGCFGSKILLTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include <utility>
#include "Config/CfgActiveSkill.h"
#include "Config/CfgFamilySkill.h"
#include "Config/CfgPassiveSkill.h"
#include "Config/CfgTalentActive.h"
#include "Config/CfgTrigSkill.h"


/**
 * @brief CfgSkillTable - Config data class
 */
class CfgSkillTable
{
public:
    CfgSkillTable();
    ~CfgSkillTable();

    void AddActiveSkill(const CfgActiveSkill& skill);
    void AddPassiveSkill(const CfgPassiveSkill& skill);
    void AddTrigSkill(const CfgTrigSkill& skill);
    void AddFamilySkill(const CfgFamilySkill& skill);
    void AddTalentActive(const CfgTalentActive& stu);
    int8_t GetSkillType(int32_t id);

    const CfgActiveSkill* GetActiveSkill(int32_t id);
    const CfgPassiveSkill* GetPassiveSkill(int32_t id);
    const CfgTrigSkill* GetTrigSkill(int32_t id);
    const CfgFamilySkill* GetFamilySkill(int32_t id);
    const CfgTalentActive* GetTalentActive(int32_t id);
    void Clear();

public:
    std::map<int, int8_t> m_mSkillType;
    std::map<int, CfgActiveSkill> m_mActiveSkills;
    std::map<int, CfgPassiveSkill> m_mPassiveSkills;
    std::map<int, CfgTrigSkill> m_mTrigSkills;
    std::map<int, CfgFamilySkill> m_mFamilySkills;
    std::map<int, CfgTalentActive> m_mTalentActive;
};

#endif // _CFGCFGSKILLTABLE_H_
