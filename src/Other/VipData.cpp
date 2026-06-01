#include "Other/VipData.h"
#include "Network/NetPacket.h"

VipData::VipData()
{
    CleanUp();
}

VipData::~VipData()
{
}

void VipData::CleanUp()
{
    m_Vip = 0;
    m_level = 0;
    m_ForeverFlg = 0;
    m_VipEndTime.clear();
    m_DropString.clear();
    m_Club = 0;
    m_ClubDropTimes = 0;
    m_BuyTimes = 0;
}

void VipData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(m_Vip);
    packet->writeInt32(m_level);
    packet->writeInt32(m_ForeverFlg);
    packet->writeUTF8(&m_VipEndTime);
    packet->writeUTF8(&m_DropString);
    packet->writeInt32(m_Club);
}

void VipData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_Vip = inPacket->readInt32();
    m_level = inPacket->readInt32();
    m_ForeverFlg = inPacket->readInt32();
    inPacket->readUTF8(&m_VipEndTime);
    inPacket->readUTF8(&m_DropString);
    m_Club = inPacket->readInt32();
}
