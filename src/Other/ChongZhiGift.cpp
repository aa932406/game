#include "Other/ChongZhiGift.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>
#include <cstdlib>

ChongZhiGift::ChongZhiGift()
    : ItemEffect()
    , nId(0)
    , Value(0)
{
}

ChongZhiGift::~ChongZhiGift()
{
}

bool ChongZhiGift::parseEffect(int32_t id, const std::string& strEffect)
{
    nId = id;
    Value = atoi(strEffect.c_str());
    return Value > 0;
}