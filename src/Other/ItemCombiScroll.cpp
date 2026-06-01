#include "Other/ItemCombiScroll.h"
#include <cstdlib>

ItemCombiScroll::ItemCombiScroll() : m_nId(0) {}
ItemCombiScroll::~ItemCombiScroll() {}
bool ItemCombiScroll::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = atoi(strEffect->c_str());
    return m_nId > 0;
}
