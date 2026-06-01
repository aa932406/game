#include "Other/VipTable.h"
#include "Other/VipCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

VipTable::VipTable()
{
}

void VipTable::InitVipTable()
{
}

const VipCfg* VipTable::GetVipCfg(int8_t VipLevel)
{
    auto it = m_VipCfgMap.find(VipLevel);
    if (it != m_VipCfgMap.end())
    {
        return &it->second;
    }
    return nullptr;
}

int8_t VipTable::GetVipLevel(int32_t VipExp)
{
    if (VipExp <= 0)
    {
        return 0;
    }
    int8_t VipLevel = 0;
    for (auto it = m_VipCfgMap.begin(); it != m_VipCfgMap.end(); ++it)
    {
        if (VipExp >= it->second.NeedVipExp)
        {
            VipLevel = it->first;
        }
        else
        {
            break;
        }
    }
    return VipLevel;
}
