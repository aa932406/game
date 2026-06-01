#include "Other/ItemEffectManager.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

ItemEffectManager::ItemEffectManager()
    : m_itemEffectsLock(nullptr)
{
}

ItemEffectManager::~ItemEffectManager()
{
}

void ItemEffectManager::init()
{
    // 初始化各类道具效果
    // CfgData *v1 = Answer::Singleton<CfgData>::instance();
    // 注册各种 ItemEffect 子类
}
