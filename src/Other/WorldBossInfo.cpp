#include "Other/WorldBossInfo.h"
#include "Network/NetPacket.h"
#include <cstring>

WorldBossInfo::WorldBossInfo()
{
    nBossId = 0;
    nLevel = 0;
    nExp = 0;
    nMid = 0;
    nBossType = 0;
    nMapId = 0;
    nReviveTime = 0;
    nState = 0;
    pPlant = 0;
    for (int32_t i = 0; i <= 4; ++i)
    {
        vKiller[i].nCharId = 0;
        vKiller[i].strName.clear();
        vKiller[i].nTime = 0;
    }
}

WorldBossInfo::WorldBossInfo(const WorldBossInfo& other)
{
    nBossId = other.nBossId;
    nLevel = other.nLevel;
    nExp = other.nExp;
    nMid = other.nMid;
    nBossType = other.nBossType;
    nMapId = other.nMapId;
    nReviveTime = other.nReviveTime;
    nState = other.nState;
    pPlant = other.pPlant;
    for (int32_t i = 0; i <= 4; ++i)
    {
        vKiller[i] = other.vKiller[i];
    }
}

WorldBossInfo::~WorldBossInfo()
{
}

WorldBossInfo& WorldBossInfo::operator=(const WorldBossInfo& other)
{
    if (this != &other)
    {
        nBossId = other.nBossId;
        nLevel = other.nLevel;
        nExp = other.nExp;
        nMid = other.nMid;
        nBossType = other.nBossType;
        nMapId = other.nMapId;
        nReviveTime = other.nReviveTime;
        nState = other.nState;
        pPlant = other.pPlant;
        for (int32_t i = 0; i <= 4; ++i)
        {
            vKiller[i] = other.vKiller[i];
        }
    }
    return *this;
}

void WorldBossInfo::PackageBossInfo(Answer::NetPacket* packet)
{
    packet->writeInt32(nBossId);
    packet->writeInt32(nLevel);
    packet->writeInt32(nExp);
    packet->writeInt32(nMid);
    packet->writeInt32(nBossType);
    packet->writeInt32(nMapId);
    packet->writeInt32(nReviveTime);
    packet->writeInt8(nState);
    for (int32_t i = 0; i <= 4; ++i)
    {
        packet->writeInt64(vKiller[i].nCharId);
        packet->writeUTF8(vKiller[i].strName);
        packet->writeInt32(vKiller[i].nTime);
    }
}

void WorldBossInfo::UnPackageBossInfo(Answer::NetPacket* inPacket)
{
    nBossId = inPacket->readInt32();
    nLevel = inPacket->readInt32();
    nExp = inPacket->readInt32();
    nMid = inPacket->readInt32();
    nBossType = inPacket->readInt32();
    nMapId = inPacket->readInt32();
    nReviveTime = inPacket->readInt32();
    nState = inPacket->readInt8();
    for (int32_t i = 0; i <= 4; ++i)
    {
        vKiller[i].nCharId = inPacket->readInt64();
        std::string str;
        inPacket->readUTF8(str);
        vKiller[i].strName = str;
        vKiller[i].nTime = inPacket->readInt32();
    }
}

void WorldBossInfo::AddKiller(const KillerInfo* info)
{
    bool bFind = false;
    for (int32_t i = 0; i <= 4; ++i)
    {
        if (!vKiller[i].nCharId)
        {
            vKiller[i] = *info;
            bFind = true;
            break;
        }
    }
    if (!bFind)
    {
        for (int32_t i = 0; i <= 3; ++i)
            vKiller[i] = vKiller[i + 1];
        vKiller[4] = *info;
    }
}
