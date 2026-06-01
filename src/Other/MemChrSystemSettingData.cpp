#include "Other/MemChrSystemSettingData.h"
#include "Network/NetPacket.h"

MemChrSystemSettingData::MemChrSystemSettingData()
{
    CleanUp();
}

MemChrSystemSettingData::~MemChrSystemSettingData()
{
}

void MemChrSystemSettingData::CleanUp()
{
    data.clear();
}

void MemChrSystemSettingData::PackageData(Answer::NetPacket *packet)
{
    packet->writeUTF8(&data);
}
