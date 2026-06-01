#include "Other/TouZiData.h"
#include "Network/NetPacket.h"

TouZiData::TouZiData()
{
    CleanUp();
}

TouZiData::~TouZiData()
{
}

void TouZiData::CleanUp()
{
    m_SevenDayTouZiTime = 0;
    m_SevenDayRecord = 0;
    m_MonthTouZiTime = 0;
    m_MonthTouZiRecord = 0;
}

void TouZiData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(static_cast<int32_t>(m_SevenDayTouZiTime));
    packet->writeInt32(m_SevenDayRecord);
    packet->writeInt32(static_cast<int32_t>(m_MonthTouZiTime));
    packet->writeInt32(m_MonthTouZiRecord);
}

void TouZiData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_SevenDayTouZiTime = inPacket->readInt32();
    m_SevenDayRecord = inPacket->readInt32();
    m_MonthTouZiTime = inPacket->readInt32();
    m_MonthTouZiRecord = inPacket->readInt32();
}
