#include "Config/CfgWingTable.h"
#include "Answer/Logger.h"
#include <map>

std::map<int32_t, int32_t> CfgWingTable::m_mWingData;

CfgWingTable::CfgWingTable()
{
    m_mWingData.clear();
}

CfgWingTable::~CfgWingTable()
{
    m_mWingData.clear();
}

void CfgWingTable::Add(int32_t nId, int32_t nAttrId)
{
    if (nId <= 0 || nAttrId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, 
            "CfgWingTable::Add invalid param: nId=%d, nAttrId=%d", nId, nAttrId);
        return;
    }
    
    m_mWingData[nId] = nAttrId;
}

int32_t CfgWingTable::GetAttrId(int32_t nId)
{
    auto it = m_mWingData.find(nId);
    if (it != m_mWingData.end())
    {
        return it->second;
    }
    return 0;
}

bool CfgWingTable::IsExist(int32_t nId)
{
    return m_mWingData.find(nId) != m_mWingData.end();
}

void CfgWingTable::Clear()
{
    m_mWingData.clear();
}