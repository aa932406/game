#include "Other/MemChrGemData.h"
#include "Network/NetPacket.h"

MemChrGemData::MemChrGemData()
{
    CleanUp();
}

MemChrGemData::~MemChrGemData()
{
}

void MemChrGemData::CleanUp()
{
    gemInfo.clear();
    posLevel.clear();
    Strengthen.clear();
    ShenYaoEquipPos.clear();
}

void MemChrGemData::PackageData(Answer::NetPacket *packet)
{
    packet->writeUTF8(&gemInfo);
    packet->writeUTF8(&posLevel);
    packet->writeUTF8(&Strengthen);
    packet->writeUTF8(&ShenYaoEquipPos);
}

void MemChrGemData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    inPacket->readUTF8(&gemInfo);
    inPacket->readUTF8(&posLevel);
    inPacket->readUTF8(&Strengthen);
    inPacket->readUTF8(&ShenYaoEquipPos);
}
