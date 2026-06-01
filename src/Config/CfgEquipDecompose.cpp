#include "Config/CfgEquipDecompose.h"

CfgEquipDecompose::CfgEquipDecompose(const CfgEquipDecompose& a2)
{
    m_nClass = a2.m_nClass;
    m_nId = a2.m_nId;
    m_nCostMoney = a2.m_nCostMoney;
    m_vGiveItems = a2.m_vGiveItems;
    m_nLimitType = a2.m_nLimitType;
    m_bDropEquip = a2.m_bDropEquip;
    m_nRateEquip = a2.m_nRateEquip;
}

CfgEquipDecompose& CfgEquipDecompose::operator=(const CfgEquipDecompose& a2)
{
    if (this != &a2)
    {
        m_nClass = a2.m_nClass;
        m_nId = a2.m_nId;
        m_nCostMoney = a2.m_nCostMoney;
        m_vGiveItems = a2.m_vGiveItems;
        m_nLimitType = a2.m_nLimitType;
        m_bDropEquip = a2.m_bDropEquip;
        m_nRateEquip = a2.m_nRateEquip;
    }
    return *this;
}
