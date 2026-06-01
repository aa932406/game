#include "Other/RechargeCard.h"
#include <cstdlib>

RechargeCard::RechargeCard() : m_addon(0) {}
RechargeCard::~RechargeCard() {}
bool RechargeCard::parseEffect(int32_t id, const std::string* strEffect)
{
    m_addon = atoi(strEffect->c_str());
    return m_addon > 0;
}
