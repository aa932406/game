#include "Other/TitleCard.h"
#include "Common/Common.h"

TitleCard::TitleCard() : m_TitleId(0), m_Time(0) {}
TitleCard::~TitleCard() {}
bool TitleCard::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 2)
        return false;
    m_TitleId = atoi(sv[0].c_str());
    m_Time = atoi(sv[1].c_str());
    return m_TitleId > 0;
}
