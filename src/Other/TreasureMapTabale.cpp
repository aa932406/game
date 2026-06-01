#include "Other/TreasureMapTabale.h"
#include "Other/TreasureMapCfg.h"
#include "Common/CommonTypes.h"
#include "Config/CfgData.h"

#include <cstring>
#include <cstdlib>
#include <algorithm>

TreasureMapTabale::TreasureMapTabale()
{
}

TreasureMapTabale::~TreasureMapTabale()
{
}

void TreasureMapTabale::AddTreasureMapCfg(TreasureMapCfg *p_stu)
{
    m_TreasureMapCfgMap[p_stu->m_ItemId] = *p_stu;
}

void TreasureMapTabale::AddTreasureMapEventCfg(TreasureMapEventCfg *p_stu)
{
    m_TreasureMapEventCfgMap[p_stu->ItemId] = *p_stu;
}

const MapPos* TreasureMapTabale::GetMapPos(int32_t ItemId)
{
    auto it = m_TreasureMapCfgMap.find(ItemId);
    if (it == m_TreasureMapCfgMap.end())
    {
        return nullptr;
    }
    const auto& cfg = it->second;
    if (cfg.m_MaxPosVector.empty() || cfg.m_MaxProbability <= 0)
    {
        return nullptr;
    }
    int32_t nRand = rand() % cfg.m_MaxProbability;
    int32_t nCumulative = 0;
    for (size_t i = 0; i < cfg.m_MaxPosVector.size(); ++i)
    {
        nCumulative += 1;
        if (nRand < nCumulative)
        {
            return &cfg.m_MaxPosVector[i];
        }
    }
    return &cfg.m_MaxPosVector.back();
}

const TreasureMapEventCfg* TreasureMapTabale::GetEvent(int32_t ItemId)
{
    auto it = m_TreasureMapCfgMap.find(ItemId);
    if (it == m_TreasureMapCfgMap.end())
    {
        return nullptr;
    }
    const auto& cfg = it->second;
    if (cfg.m_EventRate.empty())
    {
        return nullptr;
    }
    int32_t nMaxRate = 0;
    for (auto it2 = cfg.m_EventRate.begin(); it2 != cfg.m_EventRate.end(); ++it2)
    {
        nMaxRate += it2->second;
    }
    if (nMaxRate <= 0)
    {
        return nullptr;
    }
    int32_t nRand = rand() % nMaxRate;
    int32_t nCumulative = 0;
    for (auto it2 = cfg.m_EventRate.begin(); it2 != cfg.m_EventRate.end(); ++it2)
    {
        nCumulative += it2->second;
        if (nRand < nCumulative)
        {
            auto it3 = m_TreasureMapEventCfgMap.find(it2->first);
            if (it3 != m_TreasureMapEventCfgMap.end())
            {
                return &it3->second;
            }
            break;
        }
    }
    return nullptr;
}
