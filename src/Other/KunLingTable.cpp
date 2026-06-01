#include "Other/KunLingTable.h"
#include "Other/KunLevelUpCfg.h"
#include "Other/LingZhuCfg.h"
#include "Other/DanTianCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

KunLingTable::KunLingTable()
{
}

KunLingTable::~KunLingTable()
{
}

const LingZhuCfg* KunLingTable::GetLingZhuCfg(int32_t nId)
{
    auto it = m_LingZhuCfgMap.find(nId);
    if (it != m_LingZhuCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

void KunLingTable::AddKunLevelUpCfg(KunLevelUpCfg *p_Stu)
{
    m_KunLevelUpCfgMap[p_Stu->nLevel] = *p_Stu;
}

void KunLingTable::AddLingZhuCfg(LingZhuCfg *p_Stu)
{
    m_LingZhuCfgMap[p_Stu->nId] = *p_Stu;
}

void KunLingTable::AddLingZhuPos(LingZhuPosCfg stu)
{
    m_LingZhuPosMap[stu.nType] = stu;
}

void KunLingTable::AddLingZhuSuit(LingZhuSuit *p_Stu)
{
    m_LingZhuSuitMap[p_Stu->nSuitLevel] = *p_Stu;
}

void KunLingTable::AddDanTianCfg(DanTianCfg *p_Stu)
{
    m_DanTianCfgMap[p_Stu->nMid] = *p_Stu;
}

void KunLingTable::AddLingZhuSkill(int32_t SuitLevel, LingZhuSkill Stu)
{
    m_LingZhuSkillMap[SuitLevel] = Stu;
}

const KunLevelUpCfg* KunLingTable::GetKunLevelUpCfg(int32_t nLevel)
{
    auto it = m_KunLevelUpCfgMap.find(nLevel);
    if (it != m_KunLevelUpCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const LingZhuPosCfg* KunLingTable::GetLingZhuPosId(int32_t nType)
{
    auto it = m_LingZhuPosMap.find(nType);
    if (it != m_LingZhuPosMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const LingZhuSuit* KunLingTable::GetLingZhuSuit(int32_t nId)
{
    auto it = m_LingZhuSuitMap.find(nId);
    if (it != m_LingZhuSuitMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const DanTianCfg* KunLingTable::GetDanTianCfg(int32_t Mid)
{
    auto it = m_DanTianCfgMap.find(Mid);
    if (it != m_DanTianCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const LingZhuSkill* KunLingTable::GetLingZhuSkill(int32_t SuitId)
{
    auto it = m_LingZhuSkillMap.find(SuitId);
    if (it != m_LingZhuSkillMap.end())
    {
        return &it->second;
    }
    return nullptr;
}
