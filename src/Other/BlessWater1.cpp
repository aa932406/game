#include "Other/BlessWater1.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>

BlessWater1::BlessWater1()
    : ItemEffect()
    , m_nId(0)
{
}

BlessWater1::~BlessWater1()
{
}

bool BlessWater1::parseEffect(int32_t id, const std::string& strEffect)
{
    m_nId = id;
    return true;
}
