#include "Config/CfgSkillLevelUpTable.h"
#include "Answer/Logger.h"

CfgSkillLevelUpTable::CfgSkillLevelUpTable()
{
    m_mSkillLevelUp.clear();
}

CfgSkillLevelUpTable::~CfgSkillLevelUpTable()
{
    m_mSkillLevelUp.clear();
}

bool CfgSkillLevelUpTable::Add(const CfgSkillLevelUp& skill)
{
    if (skill.nSkillId <= 0 || skill.nLevel <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSkillLevelUpTable::Add invalid param: skillId=%d, level=%d", 
            skill.nSkillId, skill.nLevel);
        return false;
    }
    
    int32_t key = (skill.nSkillId << 16) | (skill.nLevel & 0xFFFF);
    m_mSkillLevelUp[key] = skill;
    return true;
}

bool CfgSkillLevelUpTable::AddSkillLevelUp(const CfgSkillLevelUp& skill)
{
    if (skill.nSkillId <= 0 || skill.nLevel <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgSkillLevelUpTable::AddSkillLevelUp invalid param: skillId=%d, level=%d", 
            skill.nSkillId, skill.nLevel);
        return false;
    }
    
    int32_t key = (skill.nSkillId << 16) | (skill.nLevel & 0xFFFF);
    m_mSkillLevelUp[key] = skill;
    return true;
}

const CfgSkillLevelUp* CfgSkillLevelUpTable::GetSkillLevelUp(int32_t nSkillId, int32_t nLevel)
{
    if (nSkillId <= 0 || nLevel <= 0)
    {
        return nullptr;
    }
    
    int32_t key = (nSkillId << 16) | (nLevel & 0xFFFF);
    auto it = m_mSkillLevelUp.find(key);
    if (it != m_mSkillLevelUp.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgSkillLevelUpTable::GetMaxLevel(int32_t nSkillId)
{
    int32_t maxLevel = 0;
    for (const auto& pair : m_mSkillLevelUp)
    {
        if (pair.second.nSkillId == nSkillId && pair.second.nLevel > maxLevel)
        {
            maxLevel = pair.second.nLevel;
        }
    }
    return maxLevel;
}

bool CfgSkillLevelUpTable::IsSkillExist(int32_t nSkillId)
{
    for (const auto& pair : m_mSkillLevelUp)
    {
        if (pair.second.nSkillId == nSkillId)
        {
            return true;
        }
    }
    return false;
}

void CfgSkillLevelUpTable::Clear()
{
    m_mSkillLevelUp.clear();
}
