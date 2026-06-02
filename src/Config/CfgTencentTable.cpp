#include "Config/CfgTencentTable.h"
#include "Answer/Logger.h"
#include <map>
#include <vector>

CfgTencentTable::CfgTencentTable()
{
    m_mTencentGift.clear();
    m_mTencentVipGift.clear();
    m_mTencentSevenDayLogin.clear();
}

CfgTencentTable::~CfgTencentTable()
{
    m_mTencentGift.clear();
    m_mTencentVipGift.clear();
    m_mTencentSevenDayLogin.clear();
}

void CfgTencentTable::Add(const CfgTencentGift& stu)
{
    if (stu.nLevel <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTencentTable::Add invalid level: %d", stu.nLevel);
        return;
    }
    
    // 根据是否有VIP奖励区分存储
    if (!stu.vVipRewards.empty())
    {
        m_mTencentVipGift[stu.nLevel] = stu;
    }
    else
    {
        m_mTencentGift[stu.nLevel] = stu;
    }
}

void CfgTencentTable::AddSevenDayLogin(const CfgTencentSevenDayLogin& stu)
{
    if (stu.nDays <= 0 || stu.nDays > 7)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTencentTable::AddSevenDayLogin invalid days: %d", stu.nDays);
        return;
    }
    
    m_mTencentSevenDayLogin[stu.nDays] = stu;
}

const CfgTencentGift* CfgTencentTable::GetGift(int32_t nLevel)
{
    auto it = m_mTencentGift.find(nLevel);
    if (it != m_mTencentGift.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgTencentGift* CfgTencentTable::GetVipGift(int32_t nLevel)
{
    auto it = m_mTencentVipGift.find(nLevel);
    if (it != m_mTencentVipGift.end())
    {
        return &it->second;
    }
    return nullptr;
}

const CfgTencentSevenDayLogin* CfgTencentTable::GetSevenDayLogin(int32_t nDays)
{
    auto it = m_mTencentSevenDayLogin.find(nDays);
    if (it != m_mTencentSevenDayLogin.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgTencentTable::GetMaxGiftLevel()
{
    if (m_mTencentGift.empty())
    {
        return 0;
    }
    return m_mTencentGift.rbegin()->first;
}

int32_t CfgTencentTable::GetMaxVipGiftLevel()
{
    if (m_mTencentVipGift.empty())
    {
        return 0;
    }
    return m_mTencentVipGift.rbegin()->first;
}

void CfgTencentTable::Clear()
{
    m_mTencentGift.clear();
    m_mTencentVipGift.clear();
    m_mTencentSevenDayLogin.clear();
}