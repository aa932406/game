#include "Other/CreateWuHun.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

CreateWuHun::CreateWuHun()
    : nId(0), SpecialCost(0)
{
}

CreateWuHun::~CreateWuHun()
{
}

CreateWuHun::CreateWuHun(const CreateWuHun& other)
    : nId(other.nId)
    , ConstItem(other.ConstItem)
    , GetItemRate(other.GetItemRate)
    , SpecialCost(other.SpecialCost)
    , GetItemRate2(other.GetItemRate2)
{
}
