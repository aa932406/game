#include "Config/CfgSkillTable.h"
#include "Answer/Logger.h"
#include <map>

std::map<int32_t, CfgActiveSkill> CfgSkillTable::m_mActiveSkills;
std::map<int32_t, CfgPassiveSkill> CfgSkillTable::m_mPassiveSkills;
std::map<int32_t, CfgTrigSkill> CfgSkillTable::m_mTrigSkills;
std::map<int32_t, CfgFamilySkill> CfgSkillTable::m_mFamilySkills;
std::map<int32_t, CfgTalentActive> CfgSkillTable::m_mTalentActive;

CfgSkillTable::CfgSkillTable()
{
    m_mActiveSkills.clear();
    m_mPassiveSkills.clear();
    m_mTrigSkills.clear();
    m_mFamilySkills.clear();
    m_mTalentActive.clear();
}

CfgSkillTable::~CfgSkillTable()
{
    m_mActiveSkills.clear();
    m_mPassiveSkills.clear();
    m_mTrigSkills.clear();
    m_mFamilySkills.clear();
    m_mTalentActive.clear();
}

void CfgSkillTable::AddActiveSkill(const CfgActiveSkill& skill)
{
    if (skill.id <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSkillTable::AddActiveSkill invalid id: %d", skill.id);
        return;
    }
    
    m_mActiveSkills[skill.id] = skill;
}

void CfgSkillTable::AddPassiveSkill(const CfgPassiveSkill& skill)
{
    if (skill.id <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSkillTable::AddPassiveSkill invalid id: %d", skill.id);
        return;
    }
    
    m_mPassiveSkills[skill.id] = skill;
}

void CfgSkillTable::AddTrigSkill(const CfgTrigSkill& skill)
{
    if (skill.id <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSkillTable::AddTrigSkill invalid id: %d", skill.id);
        return;
    }
    
    m_mTrigSkills[skill.id] = skill;
}

void CfgSkillTable::AddFamilySkill(const CfgFamilySkill& skill)
{
    if (skill.nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSkillTable::AddFamilySkill invalid id: %d", skill.nId);
        return;
    }
    
    m_mFamilySkills[skill.nId] = skill;
}

void CfgSkillTable::AddTalentActive(const CfgTalentActive& stu)
{
    if (stu.nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSkillTable::AddTalentActive invalid id: %d", stu.nId);
        return;
    }
    
    m_mTalentActive[stu.nId] = stu;
}

int8_t CfgSkillTable::GetSkillType(int32_t id)
{
    if (m_mActiveSkills.find(id) != m_mActiveSkills.end())
    {
        return 1;  // 主动技能
    }
    if (m_mPassiveSkills.find(id) != m_mPassiveSkills.end())
    {
        return 2;  // 被动技能
    }
    if (m_mTrigSkills.find(id) != m_mTrigSkills.end())
    {
        return 3;  // 触发技能
    }
    if (m_mFamilySkills.find(id) != m_mFamilySkills.end())
    {
        return 4;  // 家族技能
    }
    if (m_mTalentActive.find(id) != m_mTalentActive.end())
    {
        return 5;  // 天赋主动技能
    }
    return 0;  // 未找到
}

const CfgActiveSkill* CfgSkillTable::GetActiveSkill(int32_t id)
{
    auto it = m_mActiveSkills.find(id);
    if (it != m_mActiveSkills.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgPassiveSkill* CfgSkillTable::GetPassiveSkill(int32_t id)
{
    auto it = m_mPassiveSkills.find(id);
    if (it != m_mPassiveSkills.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgTrigSkill* CfgSkillTable::GetTrigSkill(int32_t id)
{
    auto it = m_mTrigSkills.find(id);
    if (it != m_mTrigSkills.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgFamilySkill* CfgSkillTable::GetFamilySkill(int32_t id)
{
    auto it = m_mFamilySkills.find(id);
    if (it != m_mFamilySkills.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgTalentActive* CfgSkillTable::GetTalentActive(int32_t id)
{
    auto it = m_mTalentActive.find(id);
    if (it != m_mTalentActive.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgSkillTable::Clear()
{
    m_mActiveSkills.clear();
    m_mPassiveSkills.clear();
    m_mTrigSkills.clear();
    m_mFamilySkills.clear();
    m_mTalentActive.clear();
}