#include "Config/CfgEquip.h"


CfgEquip::CfgEquip(const CfgEquip& a2)
{
    m_nId = a2.m_nId;
    m_nType = a2.m_nType;
    m_nGrade = a2.m_nGrade;
    m_nLevel = a2.m_nLevel;
    m_nJob = a2.m_nJob;
    m_nQuality = a2.m_nQuality;
    m_nSuitId = a2.m_nSuitId;
    m_nSuitId2 = a2.m_nSuitId2;
    m_nPrice = a2.m_nPrice;
    m_vAttrAddon = a2.m_vAttrAddon;
    m_vElement = a2.m_vElement;
    m_DropLuck = a2.m_DropLuck;
    m_DropRate = a2.m_DropRate;
    m_BackType = a2.m_BackType;
    m_BackValue = a2.m_BackValue;
    m_nBroadcast = a2.m_nBroadcast;
    m_backIndex = a2.m_backIndex;
    m_nLimitStar = a2.m_nLimitStar;
    m_BackGold = a2.m_BackGold;
    m_BaseAttr = a2.m_BaseAttr;
    m_PolishLevel = a2.m_PolishLevel;
    m_MoFuHuiShou = a2.m_MoFuHuiShou;
    m_SellDay = a2.m_SellDay;
    m_GongMingLevel = a2.m_GongMingLevel;
    m_WingEquipRefiningLevel = a2.m_WingEquipRefiningLevel;
    m_XinMoExp = a2.m_XinMoExp;
    m_XinMoBag = a2.m_XinMoBag;
    m_CanRongHe = a2.m_CanRongHe;
    m_TeJieParam = a2.m_TeJieParam;
}

CfgEquip& CfgEquip::operator=(const CfgEquip& a2)
{
    if (this != &a2)
    {
        m_nId = a2.m_nId;
        m_nType = a2.m_nType;
        m_nGrade = a2.m_nGrade;
        m_nLevel = a2.m_nLevel;
        m_nJob = a2.m_nJob;
        m_nQuality = a2.m_nQuality;
        m_nSuitId = a2.m_nSuitId;
        m_nSuitId2 = a2.m_nSuitId2;
        m_nPrice = a2.m_nPrice;
        m_vAttrAddon = a2.m_vAttrAddon;
        m_vElement = a2.m_vElement;
        m_DropLuck = a2.m_DropLuck;
        m_DropRate = a2.m_DropRate;
        m_BackType = a2.m_BackType;
        m_BackValue = a2.m_BackValue;
        m_nBroadcast = a2.m_nBroadcast;
        m_backIndex = a2.m_backIndex;
        m_nLimitStar = a2.m_nLimitStar;
        m_BackGold = a2.m_BackGold;
        m_BaseAttr = a2.m_BaseAttr;
        m_PolishLevel = a2.m_PolishLevel;
        m_MoFuHuiShou = a2.m_MoFuHuiShou;
        m_SellDay = a2.m_SellDay;
        m_GongMingLevel = a2.m_GongMingLevel;
        m_WingEquipRefiningLevel = a2.m_WingEquipRefiningLevel;
        m_XinMoExp = a2.m_XinMoExp;
        m_XinMoBag = a2.m_XinMoBag;
        m_CanRongHe = a2.m_CanRongHe;
        m_TeJieParam = a2.m_TeJieParam;
    }
    return *this;
}
