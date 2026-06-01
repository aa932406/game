#include "Other/MingGeExp.h"
#include <cstdlib>

MingGeExp::MingGeExp()
    : m_nId(0)
    , m_Chip(0)
{
}

MingGeExp::~MingGeExp()
{
}

bool MingGeExp::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    m_Chip = atoi(strEffect->c_str());
    return m_Chip > 0;
}
