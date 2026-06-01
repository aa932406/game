#include "Other/MGLog.h"
#include "Network/NetPacket.h"

void MGLog::PackageData(Answer::NetPacket *packet) const
{
    if (packet)
    {
        Answer::NetPacket::writeInt64(packet, Cid);
        Answer::NetPacket::writeInt32(packet, MingGeId);
        Answer::NetPacket::writeInt8(packet, Flag);
        Answer::NetPacket::writeInt32(packet, BagType);
        Answer::NetPacket::writeInt32(packet, Reason);
        Answer::NetPacket::writeInt32(packet, Time);
    }
}
