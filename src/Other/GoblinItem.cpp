#include "Other/GoblinItem.h"
#include "Common/Common.h"

GoblinItem::GoblinItem() : m_Type(0), m_MapId(0), m_X(0), m_Y(0), m_Range(0) {}
GoblinItem::~GoblinItem() {}
bool GoblinItem::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 1)
        return false;
    m_Type = atoi(sv[0].c_str());
    if (sv.size() >= 2) m_MapId = atoi(sv[1].c_str());
    if (sv.size() >= 3) m_X = atoi(sv[2].c_str());
    if (sv.size() >= 4) m_Y = atoi(sv[3].c_str());
    if (sv.size() >= 5) m_Range = atoi(sv[4].c_str());
    return m_Type > 0;
}
