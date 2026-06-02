#include "Config/CfgShouChongTable.h"
#include "Answer/Logger.h"
#include <map>

CfgShouChongTable::CfgShouChongTable()
{
    m_mShouChong.clear();
}

CfgShouChongTable::~CfgShouChongTable()
{
    m_mShouChong.clear();
}

void CfgShouChongTable::Add(const CfgShouChong& stu)
{
    if (stu.nIndex <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgShouChongTable::Add invalid index: %d", stu.nIndex);
        return;
    }
    
    m_mShouChong[stu.nIndex] = stu;
}

const CfgShouChong* CfgShouChongTable::GetShouChong(int32_t nIndex)
{
    auto it = m_mShouChong.find(nIndex);
    if (it != m_mShouChong.end())
    {
        return &it->second;
    }
    return nullptr;
}

int32_t CfgShouChongTable::GetMaxIndex()
{
    if (m_mShouChong.empty())
    {
        return 0;
    }
    return m_mShouChong.rbegin()->first;
}

bool CfgShouChongTable::IsExist(int32_t nIndex)
{
    return m_mShouChong.find(nIndex) != m_mShouChong.end();
}

void CfgShouChongTable::Clear()
{
    m_mShouChong.clear();
}