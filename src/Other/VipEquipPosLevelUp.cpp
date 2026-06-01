#include "Other/VipEquipPosLevelUp.h"

#include <cstring>
#include <algorithm>

VipEquipPosLevelUp::VipEquipPosLevelUp()
    : nSlot(0)
    , nLevel(0)
    , nRate(0)
    , nGongGaoId(0)
{
}

VipEquipPosLevelUp::~VipEquipPosLevelUp()
{
}

VipEquipPosLevelUp::VipEquipPosLevelUp(const VipEquipPosLevelUp& other)
    : nSlot(other.nSlot)
    , nLevel(other.nLevel)
    , nRate(other.nRate)
    , lAddAttrs(other.lAddAttrs)
    , lCostItems(other.lCostItems)
    , nGongGaoId(other.nGongGaoId)
{
}
