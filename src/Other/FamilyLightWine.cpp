#include "Other/FamilyLightWine.h"
#include "Common/Common.h"
#include "Common/CfgData.h"

FamilyLightWine::FamilyLightWine() : m_itemid(0)
{
    for (int i = 0; i < 6; ++i)
        m_cfgBuff[i] = 0;
}
FamilyLightWine::~FamilyLightWine() {}
bool FamilyLightWine::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 2)
        return false;
    m_itemid = atoi(sv[0].c_str());
    int32_t buffId = atoi(sv[1].c_str());
    CfgData::GetCfgDataByID(BUFF_CFG, buffId, m_cfgBuff, 6);
    return m_itemid > 0;
}
