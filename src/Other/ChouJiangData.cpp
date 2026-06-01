#include "Other/ChouJiangData.h"
#include "Network/NetPacket.h"
#include <cstring>

ChouJiangData::ChouJiangData()
{
    CleanUp();
}

ChouJiangData::~ChouJiangData()
{
}

void ChouJiangData::CleanUp()
{
    m_RecordList.clear();
    m_LuckyPoint = 0;
    m_Score = 0;
    memset(m_ItemList, 0, sizeof(m_ItemList));
}

void ChouJiangData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(m_LuckyPoint);
    packet->writeInt32(m_Score);

    int32_t slotSaveOffset = packet->getWOffset();
    int32_t slotCount = 0;
    packet->writeInt32(0);
    for (int32_t i = 0; i < 490; ++i)
    {
        int32_t itemId = *reinterpret_cast<int32_t*>(m_ItemList + i * 32 + 16);
        int32_t itemCount = *reinterpret_cast<int32_t*>(m_ItemList + i * 32 + 24);
        if (itemId > 0 && itemCount > 0)
        {
            ++slotCount;
            packet->writeInt32(i);
            packet->writeInt32(itemId);
            packet->writeInt8(*reinterpret_cast<int8_t*>(m_ItemList + i * 32 + 20));
            packet->writeInt32(itemCount);
            packet->writeInt8(*reinterpret_cast<int8_t*>(m_ItemList + i * 32 + 28));
            packet->writeInt32(*reinterpret_cast<int32_t*>(m_ItemList + i * 32 + 0));
            packet->writeInt64(*reinterpret_cast<int64_t*>(m_ItemList + i * 32 + 4));
        }
    }
    int32_t newOffset = packet->getWOffset();
    packet->setWOffset(slotSaveOffset);
    packet->writeInt32(slotCount);
    packet->setWOffset(newOffset);

    packet->writeInt32(static_cast<int32_t>(m_RecordList.size()));
    for (auto &it : m_RecordList)
    {
        packet->writeInt32(it.id);
        packet->writeInt32(it.time);
    }
}

void ChouJiangData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_LuckyPoint = inPacket->readInt32();
    m_Score = inPacket->readInt32();

    int32_t slotCount = inPacket->readInt32();
    for (int32_t i = 0; i < slotCount; ++i)
    {
        int32_t index = inPacket->readInt32();
        if (index >= 0 && index < 490)
        {
            int32_t itemId = inPacket->readInt32();
            int8_t itemClass = inPacket->readInt8();
            int32_t itemCount = inPacket->readInt32();
            int8_t bind = inPacket->readInt8();
            int32_t endTime = inPacket->readInt32();
            int64_t srcId = inPacket->readInt64();

            char *slot = m_ItemList + index * 32;
            *reinterpret_cast<int32_t*>(slot + 16) = itemId;
            *reinterpret_cast<int8_t*>(slot + 20) = itemClass;
            *reinterpret_cast<int32_t*>(slot + 24) = itemCount;
            *reinterpret_cast<int8_t*>(slot + 28) = bind;
            *reinterpret_cast<int32_t*>(slot + 0) = endTime;
            *reinterpret_cast<int64_t*>(slot + 4) = srcId;
        }
    }

    m_RecordList.clear();
    int32_t recordCount = inPacket->readInt32();
    for (int32_t j = 0; j < recordCount; ++j)
    {
        ChouJiangRecord rec;
        rec.id = inPacket->readInt32();
        rec.time = inPacket->readInt32();
        m_RecordList.push_back(rec);
    }
}