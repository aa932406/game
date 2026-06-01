#include "Other/XinMoExp.h"
#include <cstdlib>

XinMoExp::XinMoExp() : m_nId(0), m_Exp(0) {}
XinMoExp::~XinMoExp() {}
bool XinMoExp::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    m_Exp = atoi(strEffect->c_str());
    return m_Exp > 0;
}
