#include "Other/MingGeChip.h"
#include <cstdlib>

MingGeChip::MingGeChip() : m_nId(0), m_Chip(0) {}
MingGeChip::~MingGeChip() {}
bool MingGeChip::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    m_Chip = atoi(strEffect->c_str());
    return m_Chip > 0;
}
