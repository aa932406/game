// CExtCharMysteryGift.cpp
#include "CExtCharMysteryGift.h"
#include <cstring>

CExtCharMysteryGift::CExtCharMysteryGift()
{
    OnCleanUp();
}

CExtCharMysteryGift::~CExtCharMysteryGift()
{
}

void CExtCharMysteryGift::OnCleanUp()
{
}

void CExtCharMysteryGift::OnDaySwitch(int32_t nDiffDays)
{
    if (getIndex() != 1 || getState())
    {
        addIndex();
        setState(0);
        SendMysteryGiftInfo(0);
    }
}

void CExtCharMysteryGift::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 886;
    procList->push_back(v3);
}

int32_t CExtCharMysteryGift::onGetGift(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (getState()) return 10002;
    return 0;
}

int32_t CExtCharMysteryGift::onRequestInfo(Answer::NetPacket* inPacket)
{
    SendMysteryGiftInfo(1);
    return 0;
}

void CExtCharMysteryGift::SendMysteryGiftInfo(bool bCheckIndex)
{
    if (!m_pPlayer) return;
}

int32_t CExtCharMysteryGift::getLeftTime()
{
    if (!m_pPlayer) return 0;
    if (getIndex() == 1) return 0;
    return 0;
}

int64_t CExtCharMysteryGift::getCondition()
{
    return 0;
}

int8_t CExtCharMysteryGift::getState()
{
    return 0;
}

void CExtCharMysteryGift::setState(int8_t nState)
{
}

int32_t CExtCharMysteryGift::getIndex()
{
    return 1;
}

void CExtCharMysteryGift::addIndex()
{
}

void CExtCharMysteryGift::broadcastGetGift(int32_t nBroadId)
{
    if (!m_pPlayer) return;
}

void CExtCharMysteryGift::OnUpdate(int64_t curTick)
{
}

void CExtCharMysteryGift::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharMysteryGift::OnSaveToDB(PlayerDBData* dbData)
{
}