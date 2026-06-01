#include "Other/FriendExpReward.h"
#include "Network/NetPacket.h"

FriendExpReward::FriendExpReward()
{
}

FriendExpReward::~FriendExpReward()
{
}

void FriendExpReward::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(static_cast<int32_t>(m_DieRecord.size()));
    for (auto &it : m_DieRecord)
    {
        packet->writeInt64(it.Cid);
        packet->writeInt32(it.KillTime);
        packet->writeInt32(it.MapId);
        packet->writeInt32(it.MonsterId);
        packet->writeUTF8(&it.Name);
    }
}

void FriendExpReward::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_DieRecord.clear();
    int32_t nSize = inPacket->readInt32();
    for (int32_t i = 0; i < nSize; ++i)
    {
        DieRecord rec;
        rec.Cid = inPacket->readInt64();
        rec.KillTime = inPacket->readInt32();
        rec.MapId = inPacket->readInt32();
        rec.MonsterId = inPacket->readInt32();
        inPacket->readUTF8(&rec.Name);
        m_DieRecord.push_back(rec);
    }
}
