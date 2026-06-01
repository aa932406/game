#include "Other/FaBaoRes.h"
#include "Common/Common.h"

FaBaoRes::FaBaoRes() : m_Type(0) {}
FaBaoRes::~FaBaoRes() {}
bool FaBaoRes::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 1)
        return false;
    m_Type = atoi(sv[0].c_str());
    m_Values.clear();
    for (size_t i = 1; i < sv.size(); ++i)
        m_Values.push_back(atoi(sv[i].c_str()));
    return m_Type > 0;
}
