// CExtCharWish.cpp
#include "CExtCharWish.h"
#include "Database/PlayerDBData.h"
#include <cstring>

CExtCharWish::CExtCharWish()
{
    OnCleanUp();
}

CExtCharWish::~CExtCharWish()
{
}

void CExtCharWish::OnCleanUp()
{
    m_lstWishs.clear();
    m_nNextId = 0;
}

void CExtCharWish::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_lstWishs = dbData->m_WishDBData.lstWishs;
        m_nNextId = m_lstWishs.size();
    }
}

void CExtCharWish::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_WishDBData.lstWishs = m_lstWishs;
    }
}

void CExtCharWish::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 951;
    ProcId_t v4 = 952;
    procList->push_back(v3);
    procList->push_back(v4);
}

int32_t CExtCharWish::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 2;
    
    switch (nProcId)
    {
        case 951: return OnGetWishInfo(inPacket);
        case 952: return OnGetWishReward(inPacket);
        case 950: return OnWish(inPacket);
        default: return 0;
    }
}

int32_t CExtCharWish::OnWish(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharWish::OnGetWishInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 2;
    sendWishInfo();
    return 0;
}

int32_t CExtCharWish::OnGetWishReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

void CExtCharWish::sendWishInfo()
{
    if (!m_pPlayer) return;
}

void CExtCharWish::SendWishIcon()
{
    if (!m_pPlayer) return;
}

void CExtCharWish::AppendWishInfo(Answer::NetPacket* packet)
{
    if (!packet) return;
}

bool CExtCharWish::canGetReward()
{
    for (auto& wish : m_lstWishs)
    {
        if (!wish.nGetReward) return 1;
    }
    return 0;
}

int32_t CExtCharWish::getLeftTime()
{
    return 0;
}

void CExtCharWish::OnUpdate(int64_t curTick)
{
}

void CExtCharWish::OnDaySwitch(int32_t nDiffDays)
{
}