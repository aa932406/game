#include "Other/LuckDropTable.h"
#include "Other/LuckDrop.h"

#include <cstring>
#include <algorithm>

LuckDropTable::LuckDropTable()
{
}

void LuckDropTable::AddLuckDrop(LuckDrop *p_stu)
{
    auto key = std::make_pair(p_stu->Type, p_stu->VipLevel);
    m_LuckDropMap[key] = *p_stu;
}

int32_t LuckDropTable::GetLuckRate(int32_t Type, int32_t VipLevel)
{
    auto key = std::make_pair(Type, VipLevel);
    auto it = m_LuckDropMap.find(key);
    if (it != m_LuckDropMap.end())
    {
        return it->second.Rate;
    }
    return 0;
}
