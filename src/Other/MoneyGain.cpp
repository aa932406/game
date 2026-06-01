#include "Other/MoneyGain.h"
#include "Common/Common.h"

MoneyGain::MoneyGain() : m_CurrecyType(0), m_CurrecyCount(0) {}
MoneyGain::~MoneyGain() {}
bool MoneyGain::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 2)
        return false;
    m_CurrecyType = atoi(sv[0].c_str());
    m_CurrecyCount = atoi(sv[1].c_str());
    return m_CurrecyType > 0 && m_CurrecyCount > 0;
}
