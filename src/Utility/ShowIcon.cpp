#include "Utility/ShowIcon.h"
#include "Network/NetPacket.h"

void ShowIcon::AppendInfo(Answer::NetPacket* packet) const
{
    Answer::NetPacket::writeInt32(packet, nId);
    Answer::NetPacket::writeInt8(packet, nState);
    Answer::NetPacket::writeInt32(packet, nLeftTime);
    Answer::NetPacket::writeInt8(packet, nIconLeft);
    Answer::NetPacket::writeInt32(packet, nIconRight);
    Answer::NetPacket::writeInt8(packet, nEffects);
}
