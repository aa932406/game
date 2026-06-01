#include "Other/MingGeItem.h"
#include <cstdlib>

MingGeItem::MingGeItem() : m_nId(0), m_Chip(0) {}
MingGeItem::~MingGeItem() {}
bool MingGeItem::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    m_Chip = atoi(strEffect->c_str());
    return m_Chip > 0;
}
