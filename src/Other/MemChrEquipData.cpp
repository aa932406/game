#include "Other/MemChrEquipData.h"
#include "Network/NetPacket.h"
#include <cstring>

MemChrEquipData::MemChrEquipData()
{
    CleanUp();
}

MemChrEquipData::~MemChrEquipData()
{
}

void MemChrEquipData::CleanUp()
{
    memset(vEquip, 0, sizeof(vEquip));
}

void MemChrEquipData::PackageData(Answer::NetPacket *packet)
{
    for (int32_t i = 0; i <= 92; ++i)
    {
        packet->writeInt32(vEquip[i].itemId);
        packet->writeInt8(vEquip[i].itemClass);
        packet->writeInt32(vEquip[i].itemCount);
        packet->writeInt8(vEquip[i].bind);
    }
}

void MemChrEquipData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    for (int32_t i = 0; i <= 92; ++i)
    {
        vEquip[i].itemId = inPacket->readInt32();
        vEquip[i].itemClass = inPacket->readInt8();
        vEquip[i].itemCount = inPacket->readInt32();
        vEquip[i].bind = inPacket->readInt8();
    }
}
