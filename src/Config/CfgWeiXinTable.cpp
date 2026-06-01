#include "Config/CfgWeiXinTable.h"
#include "Answer/Logger.h"
#include <map>

static std::map<int32_t, CfgWeiXingGift> m_mWeiXinGift;

CfgWeiXinTable::CfgWeiXinTable()
{
    m_mWeiXinGift.clear();
}

CfgWeiXinTable::~CfgWeiXinTable()
{
    m_mWeiXinGift.clear();
}

void CfgWeiXinTable::Add(const CfgWeiXingGift& stu)
{
    if (stu.nIconId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgWeiXinTable::Add invalid icon id: %d", stu.nIconId);
        return;
    }
    
    m_mWeiXinGift[stu.nIconId] = stu;
}

const CfgWeiXingGift* CfgWeiXinTable::GetGift(int32_t nIconId)
{
    auto it = m_mWeiXinGift.find(nIconId);
    if (it != m_mWeiXinGift.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgWeiXinTable::IsGiftExist(int32_t nIconId)
{
    return m_mWeiXinGift.find(nIconId) != m_mWeiXinGift.end();
}

int32_t CfgWeiXinTable::GetRewardCount(int32_t nIconId)
{
    const CfgWeiXingGift* pGift = GetGift(nIconId);
    if (pGift)
    {
        return static_cast<int32_t>(pGift->vReward.size());
    }
    return 0;
}

void CfgWeiXinTable::Clear()
{
    m_mWeiXinGift.clear();
}