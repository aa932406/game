#include "Other/cfgEquipUpPhase.h"

#include "Other/ItemData.h"

#include <cstring>
#include <algorithm>

cfgEquipUpPhase::cfgEquipUpPhase(const cfgEquipUpPhase& a2)
{
    this->m_CostEquip = a2.m_CostEquip;
    this->m_CostItems = a2.m_CostItems;
    this->m_CostMoney = a2.m_CostMoney;
    this->m_GiveEquip = a2.m_GiveEquip;
    this->m_GongGaoId = a2.m_GongGaoId;
}

