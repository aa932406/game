#include "Other/ShiZhuang.h"
#include "Common/Common.h"
#include "Common/CfgData.h"

ShiZhuang::ShiZhuang() : m_nId(0), m_nShiZhuangId(0), m_nTime(0) {}
ShiZhuang::~ShiZhuang() {}
bool ShiZhuang::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 2)
        return false;
    m_nShiZhuangId = atoi(sv[0].c_str());
    m_nTime = atoi(sv[1].c_str());
    m_nId = id;
    return m_nShiZhuangId > 0;
}
