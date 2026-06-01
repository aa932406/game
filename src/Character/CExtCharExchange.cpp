// CExtCharExchange.cpp
#include "CExtCharExchange.h"
#include <cstring>

CExtCharExchange::CExtCharExchange()
{
    OnCleanUp();
}

CExtCharExchange::~CExtCharExchange()
{
}

void CExtCharExchange::OnCleanUp()
{
    m_lstExchangeRecords.clear();
}

void CExtCharExchange::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_lstExchangeRecords = dbData->m_ExchangeDBData.lstExchange;
    }
}

void CExtCharExchange::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_ExchangeDBData.lstExchange = m_lstExchangeRecords;
    }
}

void CExtCharExchange::OnDaySwitch(int32_t nDiffDays)
{
    if (!m_lstExchangeRecords.empty())
    {
        m_lstExchangeRecords.clear();
        for (int8_t nType = 0; nType <= 4; ++nType)
        {
            sendExchangeInfo(nType);
        }
    }
}

void CExtCharExchange::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 891;
    procList->push_back(v3);
}

int32_t CExtCharExchange::onAskExchangeInfo(Answer::NetPacket* inPacket)
{
    int8_t nType = 0;
    
    if (!m_pPlayer || !inPacket) return 10002;
    
    return 0;
}

int32_t CExtCharExchange::onExchange(Answer::NetPacket* inPacket)
{
    int32_t v2 = 0;
    int8_t nType = 0;
    int16_t nIndex = 0;
    
    if (!m_pPlayer || !inPacket) return 10002;
    
    return 0;
}

void CExtCharExchange::sendExchangeInfo(int8_t nType)
{
    if (!m_pPlayer) return;
}

int32_t CExtCharExchange::getExchangeRecord(int8_t nType, int16_t nIndex)
{
    for (auto& record : m_lstExchangeRecords)
    {
        if (record.nType == nType && record.nIndex == nIndex)
            return record.nCount;
    }
    return 0;
}

void CExtCharExchange::addExchangeRecord(int8_t nType, int16_t nIndex, int32_t nCount)
{
    for (auto& record : m_lstExchangeRecords)
    {
        if (record.nType == nType && record.nIndex == nIndex)
        {
            record.nCount += nCount;
            return;
        }
    }
    
    ExchangeRecord record;
    record.nType = nType;
    record.nIndex = nIndex;
    record.nCount = nCount;
    m_lstExchangeRecords.push_back(record);
}

void CExtCharExchange::OnUpdate(int64_t curTick)
{
}