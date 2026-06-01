#include "Other/CycleTowerTable.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

CycleTowerTable::CycleTowerTable()
{
}

CycleTowerTable::~CycleTowerTable()
{
}

void CycleTowerTable::AddCycleTowerMapCfg(int32_t MapId, CycleTowerMapCfg stu)
{
    m_CycleTowerMapCfgMap[MapId] = stu;
}

void CycleTowerTable::AddCycleTowerCost(int32_t Times, CycleTowerCost stu)
{
    m_CycleTowerCostMap[Times] = stu;
}

int32_t CycleTowerTable::GetMapId(int32_t Floor)
{
    auto it = m_CycleTowerFloor.find(Floor);
    if (it != m_CycleTowerFloor.end())
    {
        return it->second;
    }
    return 0;
}

const CycleTowerMapCfg* CycleTowerTable::GetFloorCfg(int32_t MapId)
{
    auto it = m_CycleTowerMapCfgMap.find(MapId);
    if (it != m_CycleTowerMapCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

ItemData CycleTowerTable::GetCostItem(int32_t Times)
{
    auto it = m_CycleTowerCostMap.find(Times);
    if (it != m_CycleTowerCostMap.end())
    {
        return it->second.CostData;
    }
    return ItemData();
}

int32_t CycleTowerTable::GetCostGold(int32_t Times)
{
    auto it = m_CycleTowerCostMap.lower_bound(Times);
    if (it != m_CycleTowerCostMap.end())
    {
        return it->second.CostGold;
    }
    return 0;
}
