#include "Other/CFlopDraw.h"
#include "Network/NetPacket.h"

CFlopDraw::CFlopDraw()
{
}

CFlopDraw::~CFlopDraw()
{
}

void CFlopDraw::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(static_cast<int32_t>(m_FlopDrawRecordMap.size()));
    for (auto &outer : m_FlopDrawRecordMap)
    {
        packet->writeInt32(outer.first);
        packet->writeInt32(static_cast<int32_t>(outer.second.size()));
        for (auto &inner : outer.second)
        {
            packet->writeInt32(inner.first);
            packet->writeInt32(inner.second);
        }
    }
}

void CFlopDraw::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_FlopDrawRecordMap.clear();
    int32_t outerSize = inPacket->readInt32();
    for (int32_t i = 0; i < outerSize; ++i)
    {
        int32_t outerKey = inPacket->readInt32();
        int32_t innerSize = inPacket->readInt32();
        Int32Int32Map innerMap;
        for (int32_t j = 0; j < innerSize; ++j)
        {
            int32_t key = inPacket->readInt32();
            int32_t val = inPacket->readInt32();
            innerMap[key] = val;
        }
        m_FlopDrawRecordMap[outerKey] = innerMap;
    }
}
