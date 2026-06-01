// CExtCurrency.cpp
#include "CExtCurrency.h"
#include <cstring>

CExtCurrency::CExtCurrency()
{
    OnCleanUp();
}

CExtCurrency::~CExtCurrency()
{
}

void CExtCurrency::OnCleanUp()
{
    m_bNeedSync = 0;
    memset(m_aCurrency, 0, sizeof(m_aCurrency));
    memset(m_aUpdateSign, 0, sizeof(m_aUpdateSign));
}

void CExtCurrency::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        memcpy(m_aCurrency, dbData->m_CurrencyData.vCurrency, sizeof(m_aCurrency));
    }
}

void CExtCurrency::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        memcpy(dbData->m_CurrencyData.vCurrency, m_aCurrency, sizeof(dbData->m_CurrencyData.vCurrency));
    }
}

void CExtCurrency::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 660;
    ProcId_t v4 = 20190;
    procList->push_back(v3);
    procList->push_back(v4);
}

int32_t CExtCurrency::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 10002;
    
    if (nProcId == 20190)
    {
        return onSocialAddCurrency(inPacket);
    }
    else if (nProcId == 20191)
    {
        return syncGold(inPacket);
    }
    else if (nProcId == 660)
    {
        return OnCurrencyDuiHuan(inPacket);
    }
    return 0;
}

int32_t CExtCurrency::onSocialAddCurrency(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCurrency::OnCurrencyDuiHuan(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCurrency::syncGold(Answer::NetPacket* inPacket)
{
    if (!inPacket) return 2;
    return 0;
}

int64_t CExtCurrency::GetMoneyBindAndNoBind()
{
    return m_aCurrency[0] + m_aCurrency[1];
}

void CExtCurrency::SendCurrencyInfo(bool bAll)
{
}

void CExtCurrency::SynCurrency()
{
    if (m_bNeedSync)
    {
        m_bNeedSync = 0;
        SendCurrencyInfo(0);
    }
}

void CExtCurrency::OnUpdate(int64_t curTick)
{
    SynCurrency();
}

void CExtCurrency::OnDaySwitch(int32_t nDiffDays)
{
}