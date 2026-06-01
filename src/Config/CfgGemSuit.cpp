#include "Config/CfgGemSuit.h"

CfgGemSuit::CfgGemSuit(const CfgGemSuit& a2)
{
    m_nLevel = a2.m_nLevel;
    m_vAttrAddon = a2.m_vAttrAddon;
}

CfgGemSuit& CfgGemSuit::operator=(const CfgGemSuit& a2)
{
    if (this != &a2)
    {
        m_nLevel = a2.m_nLevel;
        m_vAttrAddon = a2.m_vAttrAddon;
    }
    return *this;
}
