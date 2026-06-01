#include "Other/ChristmasDuiHuan.h"
#include "Common/CommonTypes.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

ChristmasDuiHuan::ChristmasDuiHuan()
    : nIndx(0), nLimitCount(0), nStartTime(0), nEndTime(0)
{
}

ChristmasDuiHuan::~ChristmasDuiHuan()
{
}

ChristmasDuiHuan::ChristmasDuiHuan(const ChristmasDuiHuan& other)
    : nIndx(other.nIndx)
    , Items(other.Items)
    , CostItems(other.CostItems)
    , nLimitCount(other.nLimitCount)
    , nStartTime(other.nStartTime)
    , nEndTime(other.nEndTime)
{
}
