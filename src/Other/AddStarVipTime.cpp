#include "Other/AddStarVipTime.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>
#include <cstdlib>

AddStarVipTime::AddStarVipTime()
    : ItemEffect()
    , Value(0)
{
}

AddStarVipTime::~AddStarVipTime()
{
}

bool AddStarVipTime::parseEffect(int32_t id, const std::string& strEffect)
{
    Value = atoi(strEffect.c_str());
    return Value > 0;
}
