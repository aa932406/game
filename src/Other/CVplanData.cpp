#include "Other/CVplanData.h"
#include "Network/NetPacket.h"

CVplanData::CVplanData()
{
    CleanUp();
}

CVplanData::~CVplanData()
{
}

void CVplanData::CleanUp()
{
    m_VplanType = 0;
    m_VplanLevel = 0;
    m_SwVipLevel = 0;
    m_YYLevel = 0;
    m_YYVipLevel = 0;
    m_YySuperLevel = 0;
    m_SgHallLevel = 0;
    m_strSGPf.clear();
}

void CVplanData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt8(m_VplanType);
    packet->writeInt32(m_VplanLevel);
    packet->writeInt32(m_SwVipLevel);
    packet->writeInt32(m_YYLevel);
    packet->writeInt32(m_YYVipLevel);
    packet->writeInt32(m_YySuperLevel);
    packet->writeInt32(m_SgHallLevel);
    packet->writeUTF8(&m_strSGPf);
}

void CVplanData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_VplanType = inPacket->readInt8();
    m_VplanLevel = inPacket->readInt32();
    m_SwVipLevel = inPacket->readInt32();
    m_YYLevel = inPacket->readInt32();
    m_YYVipLevel = inPacket->readInt32();
    m_YySuperLevel = inPacket->readInt32();
    m_SgHallLevel = inPacket->readInt32();
    inPacket->readUTF8(&m_strSGPf);
}
