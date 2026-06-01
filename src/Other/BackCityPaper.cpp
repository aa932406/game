#include "Other/BackCityPaper.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>

BackCityPaper::BackCityPaper(bool bXinMo)
    : ItemEffect()
    , m_BXinMo(bXinMo)
    , m_itemid(0)
{
}

BackCityPaper::~BackCityPaper()
{
}

bool BackCityPaper::parseEffect(int32_t id, const std::string& strEffect)
{
    m_itemid = id;
    return true;
}
