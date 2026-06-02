#include "Config/CfgTouZiTable.h"
#include "Answer/Logger.h"
#include <map>

static std::map<int32_t, SevenTouZi> m_mSevenTouZi;   // 七日投资 [nId]
static std::map<int32_t, MonthTouZi> m_mMonthTouZi;   // 月投资 [nDay]

CfgTouZiTable::CfgTouZiTable()
{
    if (this)
    {
        m_mSevenTouZi.clear();
        m_mMonthTouZi.clear();
    }
}

CfgTouZiTable::~CfgTouZiTable()
{
    m_mSevenTouZi.clear();
    m_mMonthTouZi.clear();
}

void CfgTouZiTable::AddSevenTouZi(SevenTouZi *p_stu)
{
    if (!this || !p_stu || p_stu->nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTouZiTable::AddSevenTouZi invalid param");
        return;
    }
    
    m_mSevenTouZi[p_stu->nId] = *p_stu;
}

void CfgTouZiTable::AddMonthTouZi(MonthTouZi *p_stu)
{
    if (!this || !p_stu || p_stu->nDay <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTouZiTable::AddMonthTouZi invalid param");
        return;
    }
    
    m_mMonthTouZi[p_stu->nDay] = *p_stu;
}

const SevenTouZi* CfgTouZiTable::GetSevenTouZi(int32_t nId)
{
    if (!this || nId <= 0)
    {
        return nullptr;
    }
    
    auto it = m_mSevenTouZi.find(nId);
    if (it != m_mSevenTouZi.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgTouZiTable::IsAllGetSevenDay(int32_t SevenDayRecord)
{
    if (!this)
    {
        return false;
    }
    
    for (const auto& pair : m_mSevenTouZi)
    {
        int32_t nId = pair.first;
        int32_t bitMask = 1 << (nId - 1);
        if ((SevenDayRecord & bitMask) == 0)
        {
            return false;
        }
    }
    return true;
}

const MonthTouZi* CfgTouZiTable::GetMonthTouZi(int32_t nDay)
{
    if (!this || nDay <= 0)
    {
        return nullptr;
    }
    
    auto it = m_mMonthTouZi.find(nDay);
    if (it != m_mMonthTouZi.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgTouZiTable::IsAllGetMonthTouZi(int32_t MonthTouZiRecord)
{
    if (!this)
    {
        return false;
    }
    
    for (const auto& pair : m_mMonthTouZi)
    {
        int32_t nDay = pair.first;
        int32_t bitMask = 1 << (nDay - 1);
        if ((MonthTouZiRecord & bitMask) == 0)
        {
            return false;
        }
    }
    return true;
}

int32_t CfgTouZiTable::GetSevenTouZiCount()
{
    if (!this)
    {
        return 0;
    }
    return static_cast<int32_t>(m_mSevenTouZi.size());
}

int32_t CfgTouZiTable::GetMonthTouZiCount()
{
    if (!this)
    {
        return 0;
    }
    return static_cast<int32_t>(m_mMonthTouZi.size());
}

void CfgTouZiTable::Clear()
{
    m_mSevenTouZi.clear();
    m_mMonthTouZi.clear();
}