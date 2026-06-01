#include "Config/CfgEquipUpPos.h"


CfgEquipUpPos::CfgEquipUpPos(const CfgEquipUpPos& a2)
{
    m_nType = a2.m_nType;
    m_nLevel = a2.m_nLevel;
    m_nRate = a2.m_nRate;
    m_nCostMoney = a2.m_nCostMoney;
    m_nNeedLevel = a2.m_nNeedLevel;
    m_vAttrAddon = a2.m_vAttrAddon;
    m_nConstItem = a2.m_nConstItem;
    m_GongGaoId = a2.m_GongGaoId;
    m_BaoHuConst = a2.m_BaoHuConst;
}

CfgEquipUpPos& CfgEquipUpPos::operator=(const CfgEquipUpPos& a2)
{
    if (this != &a2)
    {
        m_nType = a2.m_nType;
        m_nLevel = a2.m_nLevel;
        m_nRate = a2.m_nRate;
        m_nCostMoney = a2.m_nCostMoney;
        m_nNeedLevel = a2.m_nNeedLevel;
        m_vAttrAddon = a2.m_vAttrAddon;
        m_nConstItem = a2.m_nConstItem;
        m_GongGaoId = a2.m_GongGaoId;
        m_BaoHuConst = a2.m_BaoHuConst;
    }
    return *this;
}
