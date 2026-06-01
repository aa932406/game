#include "Other/NationalDayData.h"
#include "Network/NetPacket.h"

NationalDayData::NationalDayData()
{
    CleanUp();
}

NationalDayData::~NationalDayData()
{
}

void NationalDayData::CleanUp()
{
    m_Level = 0;
    m_Exp = 0;
    m_UnLockReward = 0;
    m_OrdinaryReward = 0;
    m_SeniorReward = 0;
    m_NationalInfo.clear();
}

void NationalDayData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(m_Level);
    packet->writeInt32(m_Exp);
    packet->writeInt32(m_UnLockReward);
    packet->writeInt32(m_OrdinaryReward);
    packet->writeInt32(m_SeniorReward);
    packet->writeUTF8(&m_NationalInfo);
}

void NationalDayData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_Level = inPacket->readInt32();
    m_Exp = inPacket->readInt32();
    m_UnLockReward = inPacket->readInt32();
    m_OrdinaryReward = inPacket->readInt32();
    m_SeniorReward = inPacket->readInt32();
    inPacket->readUTF8(&m_NationalInfo);
}
