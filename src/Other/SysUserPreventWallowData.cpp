#include "Other/SysUserPreventWallowData.h"
#include "Network/NetPacket.h"
#include <cstring>
#include <cstdio>

SysUserPreventWallowData::SysUserPreventWallowData()
{
    CleanUp();
}

SysUserPreventWallowData::~SysUserPreventWallowData()
{
}

void SysUserPreventWallowData::CleanUp()
{
    memset(&data, 0, sizeof(data));
}

void SysUserPreventWallowData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt64(data.uid);
    packet->writeInt32(data.sid);
    packet->writeUTF8(data.name);
    packet->writeUTF8(data.identitycard);
    packet->writeInt32(data.isGrowUp);
}

void SysUserPreventWallowData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    data.uid = inPacket->readInt64();
    data.sid = inPacket->readInt32();

    std::string tmp;
    inPacket->readUTF8(tmp);
    snprintf(data.name, sizeof(data.name), "%s", tmp.c_str());
    inPacket->readUTF8(tmp);
    snprintf(data.identitycard, sizeof(data.identitycard), "%s", tmp.c_str());

    data.isGrowUp = inPacket->readInt32();
}