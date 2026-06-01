#include "Config/CfgEquipPosSuit.h"

CfgEquipPosSuit::CfgEquipPosSuit(const CfgEquipPosSuit& a2)
{
    m_nLevel = a2.m_nLevel;
    m_vAttrAddon = a2.m_vAttrAddon;
}

CfgEquipPosSuit& CfgEquipPosSuit::operator=(const CfgEquipPosSuit& a2)
{
    if (this != &a2)
    {
        m_nLevel = a2.m_nLevel;
        m_vAttrAddon = a2.m_vAttrAddon;
    }
    return *this;
}
