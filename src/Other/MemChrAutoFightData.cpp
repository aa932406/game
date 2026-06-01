#include "Other/MemChrAutoFightData.h"
#include "Network/NetPacket.h"
#include <cstring>

MemChrAutoFightData::MemChrAutoFightData()
{
    CleanUp();
}

MemChrAutoFightData::~MemChrAutoFightData()
{
}

void MemChrAutoFightData::CleanUp()
{
    memset(data, 0, sizeof(data));
}

void MemChrAutoFightData::PackageData(Answer::NetPacket *packet)
{
    std::string val(data);
    packet->writeUTF8(&val);
}
