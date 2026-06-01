#include "Other/GongMingZhi.h"
#include <cstdlib>

GongMingZhi::GongMingZhi() : m_nId(0), m_Chip(0) {}
GongMingZhi::~GongMingZhi() {}
bool GongMingZhi::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    m_Chip = atoi(strEffect->c_str());
    return m_Chip > 0;
}
