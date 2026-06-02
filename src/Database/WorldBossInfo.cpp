#include "Database/WorldBossInfo.h"
#include "Answer/NetPacket.h"

WorldBossInfo::WorldBossInfo()
    : nBossId(0)
    , nLevel(0)
    , nExp(0)
    , nMid(0)
    , nBossType(0)
    , nMapId(0)
    , nReviveTime(0)
    , nState(0)
    , pPlant(nullptr)
{
}

void WorldBossInfo::PackageBossInfo(Answer::NetPacket *packet) const
{
    Answer::NetPacket::writeInt32(packet, nBossId);
    Answer::NetPacket::writeInt32(packet, nLevel);
    Answer::NetPacket::writeInt32(packet, nExp);
    Answer::NetPacket::writeInt32(packet, nMid);
    Answer::NetPacket::writeInt32(packet, nBossType);
    Answer::NetPacket::writeInt32(packet, nMapId);
    Answer::NetPacket::writeInt32(packet, nReviveTime);
    Answer::NetPacket::writeInt8(packet, nState);
    for (int i = 0; i < 5; ++i)
    {
        Answer::NetPacket::writeInt64(packet, vKiller[i].nCharId);
        Answer::NetPacket::writeUTF8(packet, &vKiller[i].strName);
        Answer::NetPacket::writeInt32(packet, vKiller[i].nTime);
    }
}

void WorldBossInfo::AddKiller(const KillerInfo *const info)
{
    if (!info) return;

    for (int i = 0; i < 5; ++i)
    {
        if (!vKiller[i].nCharId)
        {
            vKiller[i] = *info;
            return;
        }
    }

    // Shift and add at the end
    for (int i = 0; i < 4; ++i)
        vKiller[i] = vKiller[i + 1];
    vKiller[4] = *info;
}

void WorldBossInfo::PackageBossInfo(Answer::NetPacket *packet, int32_t nNowTime) const
{
    Answer::NetPacket::writeInt32(packet, nBossId);
    Answer::NetPacket::writeInt32(packet, nMid);
    Answer::NetPacket::writeInt32(packet, nMapId);
    Answer::NetPacket::writeInt8(packet, nState);

    int32_t nReviveLeft = nState ? (nReviveTime - nNowTime) : 0;
    Answer::NetPacket::writeInt32(packet, nReviveLeft);

    int32_t nCount = 0;
    int32_t nOldOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::writeInt32(packet, 0);

    for (int i = 0; i < 5; ++i)
    {
        if (vKiller[i].nCharId > 0)
        {
            Answer::NetPacket::writeInt64(packet, vKiller[i].nCharId);
            Answer::NetPacket::writeUTF8(packet, &vKiller[i].strName);
            Answer::NetPacket::writeInt32(packet, vKiller[i].nTime);
            ++nCount;
        }
    }

    Answer::NetPacket::writeInt32(packet, nLevel);
    Answer::NetPacket::writeInt32(packet, nExp);

    int32_t nNewOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nOldOffset);
    Answer::NetPacket::writeInt32(packet, nCount);
    Answer::NetPacket::setWOffset(packet, nNewOffset);
}
