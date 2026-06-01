#include "Other/ItemCombiPoint.h"
#include <cstdlib>

ItemCombiPoint::ItemCombiPoint() : m_nValue(0) {}
ItemCombiPoint::~ItemCombiPoint() {}
bool ItemCombiPoint::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nValue = atoi(strEffect->c_str());
    return m_nValue > 0;
}
