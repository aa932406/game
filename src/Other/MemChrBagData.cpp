#include "Other/MemChrBagData.h"
#include "Network/NetPacket.h"
#include <cstring>

MemChrBagData::MemChrBagData()
{
    CleanUp();
}

MemChrBagData::~MemChrBagData()
{
}

void MemChrBagData::CleanUp()
{
    memset(bagData, 0, sizeof(bagData));
    m_ItemLimit.clear();
}

void MemChrBagData::PackageData(Answer::NetPacket *packet)
{
    uint32_t nCount = packet->getWOffset();
    int32_t slotCount = 0;
    packet->writeInt32(0);

    int32_t nSize = sizeof(bagData) / sizeof(bagData[0]);
    for (int32_t slot = 0; slot < nSize; ++slot)
    {
        if (bagData[slot].itemId > 0)
        {
            ++slotCount;
            packet->writeInt32(slot);
            packet->writeInt32(bagData[slot].itemId);
            packet->writeInt8(bagData[slot].itemClass);
            packet->writeInt32(bagData[slot].itemCount);
            packet->writeInt8(bagData[slot].bind);
        }
    }

    int oldOffset = packet->getWOffset();
    packet->setWOffset(nCount);
    packet->writeInt32(slotCount);
    packet->setWOffset(oldOffset);

    packet->writeInt32(static_cast<int32_t>(m_ItemLimit.size()));
    for (auto &it : m_ItemLimit)
    {
        packet->writeInt32(it.first);
        packet->writeInt32(it.second);
    }
}

void MemChrBagData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    int32_t slotCount = inPacket->readInt32();
    for (int32_t i = 0; i < slotCount; ++i)
    {
        int32_t slot = inPacket->readInt32();
        if (slot >= 0 && slot < static_cast<int32_t>(sizeof(bagData) / sizeof(bagData[0])))
        {
            bagData[slot].itemId = inPacket->readInt32();
            bagData[slot].itemClass = inPacket->readInt8();
            bagData[slot].itemCount = inPacket->readInt32();
            bagData[slot].bind = inPacket->readInt8();
        }
    }

    int32_t limitCount = inPacket->readInt32();
    for (int32_t i = 0; i < limitCount; ++i)
    {
        int32_t key = inPacket->readInt32();
        int32_t val = inPacket->readInt32();
        m_ItemLimit[key] = val;
    }
}
