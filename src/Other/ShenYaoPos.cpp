#include "Other/ShenYaoPos.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

ShenYaoPos::ShenYaoPos()
    : nSlotId(0), nLevel(0), nGongGaoId(0)
{
}

ShenYaoPos::~ShenYaoPos()
{
}

ShenYaoPos::ShenYaoPos(const ShenYaoPos& other)
    : nSlotId(other.nSlotId)
    , nLevel(other.nLevel)
    , vAttrAddon(other.vAttrAddon)
    , nCost(other.nCost)
    , nGongGaoId(other.nGongGaoId)
{
}
