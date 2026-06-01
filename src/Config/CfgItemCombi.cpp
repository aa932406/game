#include "Config/CfgItemCombi.h"


CfgItemCombi::CfgItemCombi(const CfgItemCombi& a2)
{
    m_nId = a2.m_nId;
    m_lCostItem = a2.m_lCostItem;
    m_vGiveItem = a2.m_vGiveItem;
    m_nMoney = a2.m_nMoney;
    m_nRate = a2.m_nRate;
    m_nTotalRate = a2.m_nTotalRate;
    m_bBroadcast = a2.m_bBroadcast;
    m_vBackItem = a2.m_vBackItem;
    m_bNeedActive = a2.m_bNeedActive;
    m_nCostPoint = a2.m_nCostPoint;
    m_vSpecialItems = a2.m_vSpecialItems;
    m_vProbability = a2.m_vProbability;
    m_nCombiType = a2.m_nCombiType;
}

CfgItemCombi& CfgItemCombi::operator=(const CfgItemCombi& a2)
{
    if (this != &a2)
    {
        m_nId = a2.m_nId;
        m_lCostItem = a2.m_lCostItem;
        m_vGiveItem = a2.m_vGiveItem;
        m_nMoney = a2.m_nMoney;
        m_nRate = a2.m_nRate;
        m_nTotalRate = a2.m_nTotalRate;
        m_bBroadcast = a2.m_bBroadcast;
        m_vBackItem = a2.m_vBackItem;
        m_bNeedActive = a2.m_bNeedActive;
        m_nCostPoint = a2.m_nCostPoint;
        m_vSpecialItems = a2.m_vSpecialItems;
        m_vProbability = a2.m_vProbability;
        m_nCombiType = a2.m_nCombiType;
    }
    return *this;
}
