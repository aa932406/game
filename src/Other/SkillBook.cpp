#include "Other/SkillBook.h"
#include "Other/Answer.h"

SkillBook::SkillBook()
{
    m_SkillId = 0;
    m_itemid = 0;
}

SkillBook::~SkillBook()
{
}

bool SkillBook::parseEffect(int32_t id, const std::string* strEffect)
{
    m_itemid = id;
    StringVector strParam;
    Answer::StringUtility::split(&strParam, strEffect, "|", 0);
    int32_t nSize = strParam.size();
    if (nSize == 1)
    {
        m_SkillId = atoi(strParam[0].c_str());
        return m_SkillId > 0;
    }
    return false;
}
