#include "Other/EquipJinHua.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

EquipJinHua::EquipJinHua()
    : EquipId(0), MaxRate(0)
{
}

EquipJinHua::~EquipJinHua()
{
}

EquipJinHua::EquipJinHua(const EquipJinHua& other)
    : EquipId(other.EquipId)
    , CostItems(other.CostItems)
    , Rate(other.Rate)
    , MaxRate(other.MaxRate)
{
}

int32_t EquipJinHua::GetParamRate()
{
    int32_t nTotalRate = 0;
    for (auto& r : Rate)
    {
        nTotalRate += r.param1;
    }
    if (nTotalRate <= 0)
        return 0;

    return MaxRate;
}