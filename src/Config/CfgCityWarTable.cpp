#include "Config/CfgCityWarTable.h"

void CfgCityWarTable::AddContReward(const CfgCityWarContReward& stu)
{
    m_lContReward.push_back(stu);
}

const CfgCityWarContReward* CfgCityWarTable::GetContReward(int32_t id)
{
    for (auto& reward : m_lContReward)
    {
        if (reward.nIndex == id)
        {
            return &reward;
        }
    }
    return nullptr;
}
