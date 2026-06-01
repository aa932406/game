#include "Other/MemAttrData.h"
#include "Network/NetPacket.h"
#include <sstream>
#include <cstring>

MemAttrData::MemAttrData()
{
    CleanUp();
}

MemAttrData::~MemAttrData()
{
}

void MemAttrData::CleanUp()
{
    memset(vAttr, 0, sizeof(vAttr));
}

void MemAttrData::PackageData(Answer::NetPacket *packet)
{
    for (int32_t i = 0; i < 50; ++i)
        packet->writeInt32(vAttr[i]);
}

void MemAttrData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    for (int32_t i = 0; i < 50; ++i)
        vAttr[i] = inPacket->readInt32();
}

std::string MemAttrData::SaveString()
{
    std::stringstream ss;
    for (int32_t i = 1; i < 50; ++i)
    {
        ss << i << ":" << vAttr[i] << "|";
    }
    return ss.str();
}