#include "Other/MemChrTaskCycleData.h"
#include "Network/NetPacket.h"

MemChrTaskCycleData::MemChrTaskCycleData()
{
    CleanUp();
}

MemChrTaskCycleData::~MemChrTaskCycleData()
{
}

void MemChrTaskCycleData::CleanUp()
{
    nFinishTimes = 0;
    nTaskId = 0;
    nState = 0;
}

void MemChrTaskCycleData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(nFinishTimes);
    packet->writeInt32(nTaskId);
    packet->writeInt8(nState);
}

void MemChrTaskCycleData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    nFinishTimes = inPacket->readInt32();
    nTaskId = inPacket->readInt32();
    nState = inPacket->readInt8();
}
