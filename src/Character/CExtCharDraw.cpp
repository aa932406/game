// CExtCharDraw.cpp
#include "CExtCharDraw.h"
#include <cstring>

CExtCharDraw::CExtCharDraw()
{
    OnCleanUp();
}

CExtCharDraw::~CExtCharDraw()
{
}

void CExtCharDraw::OnCleanUp()
{
}

void CExtCharDraw::GetInterestsProtocol(ProcIdList* procList)
{
}

int32_t CExtCharDraw::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 2;
    if (nProcId == 920) return OnDraw(inPacket);
    return 0;
}

int32_t CExtCharDraw::OnDraw(Answer::NetPacket* inPacket)
{
    int32_t v2 = 0;
    int8_t nType = 0;
    
    if (!inPacket || !m_pPlayer) return 2;
    
    return 0;
}

int32_t CExtCharDraw::GetItemRecord(int8_t nType)
{
    if (!m_pPlayer) return 0;
    
    switch (nType)
    {
        case 1: return 0;
        case 2: return 0;
        case 3: return 0;
        case 4: return 0;
        default: return 0;
    }
}

bool CExtCharDraw::canDraw(int8_t nType)
{
    if (!m_pPlayer) return 0;
    
    switch (nType)
    {
        case 1: return 1;
        case 2: return 1;
        case 3: return 1;
        case 4: return 1;
        default: return 0;
    }
}

void CExtCharDraw::addDrawTimes(int8_t nType, int32_t GetItemRecord)
{
}

void CExtCharDraw::OnUpdate(int64_t curTick)
{
}

void CExtCharDraw::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharDraw::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharDraw::OnSaveToDB(PlayerDBData* dbData)
{
}