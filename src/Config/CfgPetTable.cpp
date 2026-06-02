#include "Config/CfgPetTable.h"
#include "Answer/Logger.h"
#include <map>

CfgPetTable::CfgPetTable()
{
    m_mPetData.clear();
}

CfgPetTable::~CfgPetTable()
{
    m_mPetData.clear();
}

void CfgPetTable::Add(const CfgPetData& stu)
{
    if (stu.m_nPetId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgPetTable::Add invalid pet id: %d", stu.m_nPetId);
        return;
    }
    
    m_mPetData[stu.m_nPetId] = stu;
}

const CfgPetData* CfgPetTable::GetPetData(int32_t nPetId)
{
    auto it = m_mPetData.find(nPetId);
    if (it != m_mPetData.end())
    {
        return &it->second;
    }
    return nullptr;
}

bool CfgPetTable::IsPetExist(int32_t nPetId)
{
    return m_mPetData.find(nPetId) != m_mPetData.end();
}

void CfgPetTable::Clear()
{
    m_mPetData.clear();
}

int32_t CfgPetTable::GetPetCount()
{
    return static_cast<int32_t>(m_mPetData.size());
}