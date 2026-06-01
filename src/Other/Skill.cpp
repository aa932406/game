#include "Other/Skill.h"

#include <cstring>
#include <algorithm>

Skill::Skill()
    : m_cfg(nullptr)
    , m_nSkillFlag(0)
{
}

Skill::Skill(const CfgActiveSkill* pCfgSkill, int32_t nSkillFlag)
    : m_cfg(pCfgSkill)
    , m_nSkillFlag(nSkillFlag)
{
}

int32_t Skill::calBaseRevive(int32_t atkMin, int32_t atkMax, int32_t sklModify)
{
    if (atkMax <= atkMin)
    {
        return atkMin * sklModify / 100;
    }
    
    Answer::Random* random = Answer::Random::GetInstance();
    int32_t atk = random->generate(atkMin, atkMax);
    return atk * sklModify / 100;
}
