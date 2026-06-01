#include "Other/cfgEquipUpPhaseTable.h"
#include "Other/cfgEquipUpPhase.h"

#include <cstring>
#include <algorithm>

cfgEquipUpPhaseTable::cfgEquipUpPhaseTable()
{
}

void cfgEquipUpPhaseTable::AddData(cfgEquipUpPhase* stu)
{
    if (stu)
    {
        m_cfgEquipUpPhase.push_back(*stu);
    }
}

const cfgEquipUpPhase* cfgEquipUpPhaseTable::GetData(int32_t key) const
{
    for (auto& data : m_cfgEquipUpPhase)
    {
        // key comparison logic based on decompiled code
        // The original iterated through the list to find matching entry
    }
    return nullptr;
}
