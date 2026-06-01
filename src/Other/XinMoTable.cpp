#include "Other/XinMoTable.h"
#include "Other/XinMoCfg.h"
#include "Other/QiQingCfg.h"
#include "Other/EquipJinHua.h"
#include "Other/XinMoQiQingLevelUpCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

XinMoTable::XinMoTable()
{
}

XinMoTable::~XinMoTable()
{
}

void XinMoTable::AddXingMoCfg(XinMoCfg *p_Stu)
{
    m_XinMoCfgMap[p_Stu->XinMoLevel] = *p_Stu;
}

void XinMoTable::AddQiQingCfg(QiQingCfg *p_Stu)
{
    m_QiQingCfgMap[p_Stu->nLevel] = *p_Stu;
}

void XinMoTable::AddEquipJinHuaCfg(EquipJinHua *p_Stu)
{
    m_EquipJinHuaMap[p_Stu->EquipId] = *p_Stu;
}

void XinMoTable::AddXinMoQiQingLevelUpCfg(XinMoQiQingLevelUpCfg *p_Stu)
{
    auto key = std::make_pair(p_Stu->nType, p_Stu->nLevel);
    m_XinMoQiQingLevelUpCfgMap[key] = *p_Stu;
}

void XinMoTable::AddXinQingCfg(XinQingCfg stu)
{
    m_XinQingCfgList.push_back(stu);
}

void XinMoTable::AddXinQingReward(int32_t nId, XinQingReward *p_stu)
{
    m_MemChrBagVectorMap[nId] = *p_stu;
}

const XinMoCfg* XinMoTable::GetXingMoCfg(int32_t nlevel)
{
    auto it = m_XinMoCfgMap.find(nlevel);
    if (it != m_XinMoCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const QiQingCfg* XinMoTable::GetQiQingCfg(int32_t nLevel)
{
    auto it = m_QiQingCfgMap.find(nLevel);
    if (it != m_QiQingCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const EquipJinHua* XinMoTable::GetJinHuaCfg(int32_t EquipId)
{
    auto it = m_EquipJinHuaMap.find(EquipId);
    if (it != m_EquipJinHuaMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t XinMoTable::GetAppearCount(int32_t nLeftCount)
{
    int32_t nAppearCount = 0;
    for (auto it = m_XinQingCfgList.begin(); it != m_XinQingCfgList.end(); ++it)
    {
        if (nLeftCount >= it->nCountMin && nLeftCount <= it->nCountMax)
        {
            nAppearCount = it->nAppearCount;
            break;
        }
    }
    return nAppearCount;
}

const XinQingReward* XinMoTable::GetXinQingReward(int32_t nId)
{
    auto it = m_MemChrBagVectorMap.find(nId);
    if (it != m_MemChrBagVectorMap.end())
    {
        return &it->second;
    }
    return nullptr;
}
