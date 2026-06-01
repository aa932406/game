#include "Config/CfgItemGem.h"


CfgItemGem::CfgItemGem(const CfgItemGem& a2)
{
    m_nId = a2.m_nId;
    m_nType = a2.m_nType;
    m_nLevel = a2.m_nLevel;
    m_nPrice = a2.m_nPrice;
    m_nQuality = a2.m_nQuality;
    m_nLayNum = a2.m_nLayNum;
    m_lAddPos = a2.m_lAddPos;
    m_vAttrAddon = a2.m_vAttrAddon;
}

CfgItemGem& CfgItemGem::operator=(const CfgItemGem& a2)
{
    if (this != &a2)
    {
        m_nId = a2.m_nId;
        m_nType = a2.m_nType;
        m_nLevel = a2.m_nLevel;
        m_nPrice = a2.m_nPrice;
        m_nQuality = a2.m_nQuality;
        m_nLayNum = a2.m_nLayNum;
        m_lAddPos = a2.m_lAddPos;
        m_vAttrAddon = a2.m_vAttrAddon;
    }
    return *this;
}
