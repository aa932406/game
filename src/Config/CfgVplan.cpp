#include "Config/CfgVplan.h"

CfgVplan::CfgVplan(const CfgVplan& a2)
{
    m_VplanRewardMap = a2.m_VplanRewardMap;
    m_VplanLevelRewardMap = a2.m_VplanLevelRewardMap;
    m_VplanEveryDayRewardMap = a2.m_VplanEveryDayRewardMap;
}

CfgVplan& CfgVplan::operator=(const CfgVplan& a2)
{
    if (this != &a2)
    {
        m_VplanRewardMap = a2.m_VplanRewardMap;
        m_VplanLevelRewardMap = a2.m_VplanLevelRewardMap;
        m_VplanEveryDayRewardMap = a2.m_VplanEveryDayRewardMap;
    }
    return *this;
}
