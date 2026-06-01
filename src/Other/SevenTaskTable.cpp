#include "Other/SevenTaskTable.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

SevenTaskTable::SevenTaskTable()
{
}

SevenTaskTable::~SevenTaskTable()
{
}

void SevenTaskTable::AddSevenTaskSumRewardMap(SevenTaskSumReward stu)
{
    m_SevenTaskSumRewardMap[stu.nId] = stu;
}

void SevenTaskTable::AddSevenTaskCfgMap(SevenTaskCfg stu)
{
    m_SevenTaskCfgMap[stu.nId] = stu;
}

const SevenTaskSumReward* SevenTaskTable::GetSevenTaskSumReward(int32_t nId)
{
    auto it = m_SevenTaskSumRewardMap.find(nId);
    if (it != m_SevenTaskSumRewardMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

const SevenTaskCfg* SevenTaskTable::GetSevenTaskCfg(int32_t nId)
{
    auto it = m_SevenTaskCfgMap.find(nId);
    if (it != m_SevenTaskCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}
