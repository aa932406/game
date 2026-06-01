#include "Config/CfgEquipUpStar.h"


CfgEquipUpStar::CfgEquipUpStar(const CfgEquipUpStar& a2)
{
    m_nType = a2.m_nType;
    m_nStar = a2.m_nStar;
    m_nRate = a2.m_nRate;
    m_nSuccessAddStar = a2.m_nSuccessAddStar;
    m_nFailLostStar = a2.m_nFailLostStar;
    m_lCosItem = a2.m_lCosItem;
    m_nCostMoney = a2.m_nCostMoney;
    m_vAttrAddon = a2.m_vAttrAddon;
    m_nCostXingMai = a2.m_nCostXingMai;
    m_RongLianAttr = a2.m_RongLianAttr;
}

CfgEquipUpStar& CfgEquipUpStar::operator=(const CfgEquipUpStar& a2)
{
    if (this != &a2)
    {
        m_nType = a2.m_nType;
        m_nStar = a2.m_nStar;
        m_nRate = a2.m_nRate;
        m_nSuccessAddStar = a2.m_nSuccessAddStar;
        m_nFailLostStar = a2.m_nFailLostStar;
        m_lCosItem = a2.m_lCosItem;
        m_nCostMoney = a2.m_nCostMoney;
        m_vAttrAddon = a2.m_vAttrAddon;
        m_nCostXingMai = a2.m_nCostXingMai;
        m_RongLianAttr = a2.m_RongLianAttr;
    }
    return *this;
}
