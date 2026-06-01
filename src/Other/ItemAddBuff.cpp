#include "Other/ItemAddBuff.h"
#include "Other/CfgData.h"
#include "Other/Answer.h"

ItemAddBuff::ItemAddBuff()
{
    m_nItemId = 0;
}

ItemAddBuff::~ItemAddBuff()
{
}

bool ItemAddBuff::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nItemId = id;
    StringVector vStr;
    Answer::StringUtility::split(&vStr, strEffect, ":", 0);
    if (vStr.empty())
        return false;
    for (size_t i = 0; i < vStr.size(); ++i)
    {
        int32_t buffId = atoi(vStr[i].c_str());
        CfgData* v7 = Answer::Singleton<CfgData>::instance();
        CfgBuff* pCfgBuff = CfgData::getBuff(v7, buffId);
        if (!pCfgBuff)
            return false;
        m_lBuffs.push_back(pCfgBuff);
    }
    return true;
}
