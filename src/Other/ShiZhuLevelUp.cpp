#include "Other/ShiZhuLevelUp.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

ShiZhuLevelUp::ShiZhuLevelUp()
    : nId(0), nLevel(0)
{
}

ShiZhuLevelUp::~ShiZhuLevelUp()
{
}

ShiZhuLevelUp::ShiZhuLevelUp(const ShiZhuLevelUp& other)
    : nId(other.nId)
    , nLevel(other.nLevel)
    , lCostItem(other.lCostItem)
    , lAttr(other.lAttr)
{
}
