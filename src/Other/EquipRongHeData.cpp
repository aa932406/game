#include "Other/EquipRongHeData.h"
#include "Network/NetPacket.h"

EquipRongHeData::EquipRongHeData()
{
    CleanUp();
}

EquipRongHeData::~EquipRongHeData()
{
}

void EquipRongHeData::CleanUp()
{
    m_EquipRongHe.clear();
}

void EquipRongHeData::PackageData(Answer::NetPacket *packet)
{
    packet->writeUTF8(&m_EquipRongHe);
}

void EquipRongHeData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    inPacket->readUTF8(&m_EquipRongHe);
}
