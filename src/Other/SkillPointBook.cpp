#include "Other/SkillPointBook.h"
#include "Common/Common.h"

SkillPointBook::SkillPointBook() : m_SkillId(0), m_SkillPoints(0), m_itemid(0) {}
SkillPointBook::~SkillPointBook() {}
bool SkillPointBook::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 2)
        return false;
    m_SkillId = atoi(sv[0].c_str());
    m_SkillPoints = atoi(sv[1].c_str());
    m_itemid = id;
    return m_SkillId > 0;
}
