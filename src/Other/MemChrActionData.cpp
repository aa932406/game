#include "Other/MemChrActionData.h"
#include "Network/NetPacket.h"
#include <cstring>

MemChrActionData::MemChrActionData()
{
    CleanUp();
}

MemChrActionData::~MemChrActionData()
{
}

void MemChrActionData::CleanUp()
{
    memset(actionArry, 0, sizeof(actionArry));
}

void MemChrActionData::PackageData(Answer::NetPacket *packet)
{
    for (int32_t i = 1; i <= 12; ++i)
    {
        packet->writeInt32(actionArry[i].id);
        packet->writeInt32(actionArry[i].type);
    }
}

void MemChrActionData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    for (int32_t i = 1; i <= 12; ++i)
    {
        actionArry[i].id = inPacket->readInt32();
        actionArry[i].type = inPacket->readInt32();
    }
}
