#include "Other/cfgGoblinTableData.h"
#include "Other/cfgGoblinData.h"

#include <cstring>
#include <algorithm>

cfgGoblinTableData::cfgGoblinTableData()
{
}

void cfgGoblinTableData::Add(cfgGoblinData* stu)
{
    if (stu)
    {
        m_GoblinDataMap[stu->m_nIndex] = *stu;
    }
}

const cfgGoblinData* cfgGoblinTableData::getGoblinData(int32_t index) const
{
    auto it = m_GoblinDataMap.find(index);
    if (it != m_GoblinDataMap.end())
    {
        return &it->second;
    }
    return nullptr;
}
