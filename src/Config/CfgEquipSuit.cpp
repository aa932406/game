#include "Config/CfgEquipSuit.h"


CfgEquipSuit::CfgEquipSuit(const CfgEquipSuit& a2)
{
    m_nId = a2.m_nId;
    m_nCount = a2.m_nCount;
    m_lstSuitAttr = a2.m_lstSuitAttr;
    m_IdList = a2.m_IdList;
    m_nBuffId = a2.m_nBuffId;
}

CfgEquipSuit& CfgEquipSuit::operator=(const CfgEquipSuit& a2)
{
    if (this != &a2)
    {
        m_nId = a2.m_nId;
        m_nCount = a2.m_nCount;
        m_lstSuitAttr = a2.m_lstSuitAttr;
        m_IdList = a2.m_IdList;
        m_nBuffId = a2.m_nBuffId;
    }
    return *this;
}
