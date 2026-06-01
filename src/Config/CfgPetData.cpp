#include "Config/CfgPetData.h"


CfgPetData::CfgPetData(const CfgPetData& a2)
{
    m_nPetId = a2.m_nPetId;
    m_Quality = a2.m_Quality;
    m_vBaseAttr = a2.m_vBaseAttr;
}

CfgPetData& CfgPetData::operator=(const CfgPetData& a2)
{
    if (this != &a2)
    {
        m_nPetId = a2.m_nPetId;
        m_Quality = a2.m_Quality;
        m_vBaseAttr = a2.m_vBaseAttr;
    }
    return *this;
}
