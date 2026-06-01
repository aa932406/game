// CExtCharAuction.cpp
#include "CExtCharAuction.h"
#include <cstring>

CExtCharAuction::CExtCharAuction()
{
    OnCleanUp();
}

CExtCharAuction::~CExtCharAuction()
{
}

void CExtCharAuction::OnCleanUp()
{
    m_nStall = 0;
}

void CExtCharAuction::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 821;
    ProcId_t v4 = 20072;
    ProcId_t v5 = 20073;
    ProcId_t v6 = 20075;
    procList->push_back(v3);
    procList->push_back(v4);
    procList->push_back(v5);
    procList->push_back(v6);
}

int32_t CExtCharAuction::onAuctionSell(Answer::NetPacket* inPacket)
{
    int32_t v2 = 0;
    int8_t nType = 0;
    int32_t nSlot = 0;
    int32_t nCount = 0;
    int32_t nPrice = 0;
    int8_t nCurrType = 0;
    MemChrBag item;
    
    if (!m_pPlayer || !inPacket) return 10002;
    
    memset(&item, 0, sizeof(item));
    
    return 0;
}

int32_t CExtCharAuction::onAuctionBuy(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    return 0;
}

int32_t CExtCharAuction::onOpenStall(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    m_nStall = 1;
    return 0;
}

int32_t CExtCharAuction::onCloseStall(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (!closeStall()) return 2;
    return 0;
}

int32_t CExtCharAuction::onAddEquip(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    return 0;
}

void CExtCharAuction::sendSocialBuy(int32_t nId)
{
    if (!m_pPlayer) return;
}

void CExtCharAuction::sendSocialStall(bool bStall)
{
    if (!m_pPlayer) return;
}

bool CExtCharAuction::closeStall()
{
    if (!m_pPlayer) return 0;
    if (!IsInStall()) return 0;
    m_nStall = 0;
    return 1;
}

void CExtCharAuction::OnUpdate(int64_t curTick)
{
}

void CExtCharAuction::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharAuction::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharAuction::OnSaveToDB(PlayerDBData* dbData)
{
}