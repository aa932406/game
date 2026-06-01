#include "Config/CfgWishRewardTable.h"
#include "Answer/Logger.h"
#include <map>
#include <vector>
#include <algorithm>

std::vector<CfgWishReward> CfgWishRewardTable::m_vWishReward;

CfgWishRewardTable::CfgWishRewardTable()
{
    m_vWishReward.clear();
}

CfgWishRewardTable::~CfgWishRewardTable()
{
    m_vWishReward.clear();
}

void CfgWishRewardTable::Add(const CfgWishReward& stu)
{
    if (stu.nTime <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgWishRewardTable::Add invalid time: %d", stu.nTime);
        return;
    }
    
    m_vWishReward.push_back(stu);
    
    // 按时间排序
    std::sort(m_vWishReward.begin(), m_vWishReward.end(),
        [](const CfgWishReward& a, const CfgWishReward& b) {
            return a.nTime < b.nTime;
        });
}

const CfgWishReward* CfgWishRewardTable::GetReward(int32_t nTime)
{
    for (const auto& reward : m_vWishReward)
    {
        if (reward.nTime == nTime)
        {
            return &reward;
        }
    }
    return nullptr;
}

const CfgWishReward* CfgWishRewardTable::GetRewardByIndex(int32_t nIndex)
{
    if (nIndex >= 0 && nIndex < static_cast<int32_t>(m_vWishReward.size()))
    {
        return &m_vWishReward[nIndex];
    }
    return nullptr;
}

int32_t CfgWishRewardTable::GetRewardCount()
{
    return static_cast<int32_t>(m_vWishReward.size());
}

int32_t CfgWishRewardTable::GetMaxTime()
{
    if (m_vWishReward.empty())
    {
        return 0;
    }
    return m_vWishReward.back().nTime;
}

int32_t CfgWishRewardTable::GetMinTime()
{
    if (m_vWishReward.empty())
    {
        return 0;
    }
    return m_vWishReward.front().nTime;
}

void CfgWishRewardTable::Clear()
{
    m_vWishReward.clear();
}