#include "Other/CurrencyRes.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>

CurrencyRes::CurrencyRes()
    : ItemEffect()
    , m_id(0)
    , m_CurrecyType(-1)
    , m_CurrecyCount(0)
{
}

CurrencyRes::~CurrencyRes()
{
}

bool CurrencyRes::parseEffect(int32_t id, const std::string& strEffect)
{
    // 解析货币类型和数量
    // 格式: type,count
    return true;
}