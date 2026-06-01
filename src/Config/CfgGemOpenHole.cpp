#include "Config/CfgGemOpenHole.h"

CfgGemOpenHole::CfgGemOpenHole(const CfgGemOpenHole& a2)
{
    m_nSlot = a2.m_nSlot;
    m_nCostMoney = a2.m_nCostMoney;
    m_lCostItem = a2.m_lCostItem;
}

CfgGemOpenHole& CfgGemOpenHole::operator=(const CfgGemOpenHole& a2)
{
    if (this != &a2)
    {
        m_nSlot = a2.m_nSlot;
        m_nCostMoney = a2.m_nCostMoney;
        m_lCostItem = a2.m_lCostItem;
    }
    return *this;
}
