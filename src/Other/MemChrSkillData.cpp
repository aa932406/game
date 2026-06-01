#include "Other/MemChrSkillData.h"
#include "Network/NetPacket.h"

MemChrSkillData::MemChrSkillData()
{
    CleanUp();
}

MemChrSkillData::~MemChrSkillData()
{
}

void MemChrSkillData::CleanUp()
{
    talents.clear();
    m_nPower = 0;
}

void MemChrSkillData::PackageData(Answer::NetPacket *packet)
{
    packet->writeUTF8(&talents);
    packet->writeInt32(m_nPower);
}

void MemChrSkillData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    inPacket->readUTF8(&talents);
    m_nPower = inPacket->readInt32();
}
