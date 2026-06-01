#include "Other/EquipBox.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>

EquipBox::EquipBox()
    : ItemEffect()
    , m_nItemId(0)
{
}

EquipBox::~EquipBox()
{
}

bool EquipBox::parseEffect(int32_t id, const std::string& strEffect)
{
    m_nItemId = id;
    // 从配置表获取装备箱子数据
    return true;
}