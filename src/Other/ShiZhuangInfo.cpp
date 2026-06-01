#include "Other/ShiZhuangInfo.h"
#include "Utility/Answer.h"

ShiZhuangInfo::ShiZhuangInfo()
{
    CleanUp();
}

void ShiZhuangInfo::CleanUp()
{
    m_nWear = 0;
    m_nLevel = 0;
    m_nExp = 0;
    m_lActive.clear();
}

void ShiZhuangInfo::ParesActiveString(std::string *strActive)
{
    int32_t Now = Answer::DayTime::now();
    m_lActive.clear();
    if (!strActive->empty())
    {
        StringVector vt;
        Answer::StringUtility::split(&vt, strActive, "|", 0);
        for (auto iter = vt.begin(); iter != vt.end(); ++iter)
        {
            StringVector ShiZhuangVt;
            Answer::StringUtility::split(&ShiZhuangVt, &*iter, ":", 0);
            if (ShiZhuangVt.size() == 2)
            {
                int32_t nId = atoi(ShiZhuangVt[0].c_str());
                int32_t nTime = atoi(ShiZhuangVt[1].c_str());
                m_lActive[nId] = nTime;
            }
        }
    }
}

std::string ShiZhuangInfo::GetActiveString()
{
    int32_t Now = Answer::DayTime::now();
    std::stringstream ss;
    for (auto iter = m_lActive.begin(); iter != m_lActive.end(); ++iter)
    {
        ss << iter->first << ":" << iter->second << "|";
    }
    return ss.str();
}

void ShiZhuangInfo::AddTime(int32_t nId, int32_t nTime)
{
    if (nTime)
    {
        int32_t Now = Answer::DayTime::now();
        if (!IsActive(nId))
        {
            m_lActive[nId] = Now + nTime;
        }
        else if (m_lActive[nId] > 0)
        {
            m_lActive[nId] += nTime;
        }
    }
    else
    {
        m_lActive[nId] = 0;
    }
}

bool ShiZhuangInfo::IsForever(int32_t nId)
{
    auto it = m_lActive.find(nId);
    return it != m_lActive.end() && it->second == 0;
}

bool ShiZhuangInfo::IsActive(int32_t nId)
{
    int32_t Now = Answer::DayTime::now();
    auto it = m_lActive.find(nId);
    if (it == m_lActive.end())
        return false;
    if (it->second)
    {
        if (it->second <= Now)
            return false;
    }
    return true;
}
