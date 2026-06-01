#include "Other/MemChrTaskData.h"
#include "Network/NetPacket.h"
#include <cstring>

MemChrTaskData::MemChrTaskData()
{
}

MemChrTaskData::~MemChrTaskData()
{
}

void MemChrTaskData::PackageData(Answer::NetPacket *packet)
{
    int32_t vtSize = static_cast<int32_t>(taskVt.size());
    packet->writeInt32(vtSize);
    for (int32_t i = 0; i < vtSize; ++i)
    {
        packet->writeInt32(taskVt[i].id);
        packet->writeInt32(taskVt[i].state);
        packet->writeInt32(taskVt[i].monster);
    }
}

void MemChrTaskData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    int32_t taskCount = inPacket->readInt32();
    taskVt.reserve(taskCount);
    for (int32_t i = 0; i < taskCount; ++i)
    {
        MemChrTask task;
        task.id = inPacket->readInt32();
        task.state = inPacket->readInt32();
        task.monster = inPacket->readInt32();
        taskVt.push_back(task);
    }
}
