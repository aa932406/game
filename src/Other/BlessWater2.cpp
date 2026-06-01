#include "Other/BlessWater2.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>

BlessWater2::BlessWater2()
    : ItemEffect()
    , m_nId(0)
{
}

BlessWater2::~BlessWater2()
{
}

bool BlessWater2::parseEffect(int32_t id, const std::string& strEffect)
{
    m_nId = id;
    return true;
}
