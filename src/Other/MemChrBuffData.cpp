#include "Other/MemChrBuffData.h"
#include "Network/NetPacket.h"
#include <cstring>

MemChrBuffData::MemChrBuffData()
{
}

MemChrBuffData::~MemChrBuffData()
{
}

void MemChrBuffData::PackageData(Answer::NetPacket *packet)
{
    int32_t vtSize = static_cast<int32_t>(buffVt.size());
    packet->writeInt32(vtSize);
    for (int32_t i = 0; i < vtSize; ++i)
    {
        packet->writeInt32(buffVt[i].buffId);
        packet->writeInt8(buffVt[i].index);
    }
}

void MemChrBuffData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    int32_t buffCount = inPacket->readInt32();
    buffVt.reserve(buffCount);
    for (int32_t i = 0; i < buffCount; ++i)
    {
        MemChrBuff buff;
        buff.buffId = inPacket->readInt32();
        buff.index = inPacket->readInt8();
        buffVt.push_back(buff);
    }
}
