#include "Other/MemEquip.h"
#include "Network/NetPacket.h"
#include <cstring>

MemEquip::MemEquip()
{
    CleanUp();
}

MemEquip::MemEquip(const MemEquip &other)
    : id(other.id)
    , base(other.base)
    , owner(other.owner)
    , star(other.star)
    , name(other.name)
    , time(other.time)
    , MapId(other.MapId)
    , Mid(other.Mid)
{
}

MemEquip::~MemEquip()
{
}

void MemEquip::CleanUp()
{
    id = 0;
    base = 0;
    owner = 0;
    star = 0;
    name.clear();
    time = 0;
    MapId = 0;
    Mid = 0;
}

void MemEquip::PackageData(Answer::NetPacket *packet) const
{
    if (!packet) return;
    packet->writeInt64(id);
    packet->writeInt32(base);
    packet->writeInt64(owner);
    packet->writeInt8(star);
    packet->writeInt32(MapId);
    packet->writeInt32(Mid);
    packet->writeInt32(time);
    packet->writeUTF8(name);
    packet->writeInt32(0);
}

void MemEquip::UnPackageData(Answer::NetPacket *inPacket)
{
    if (!inPacket) return;
    id = inPacket->readInt64();
    base = inPacket->readInt32();
    owner = inPacket->readInt64();
    star = inPacket->readInt8();
    MapId = inPacket->readInt32();
    Mid = inPacket->readInt32();
    time = inPacket->readInt32();
    inPacket->readUTF8(name);
    inPacket->readInt32();
}

void MemEquip::PackageClientData(Answer::NetPacket *packet) const
{
    if (!packet) return;
    packet->writeInt64(id);
    packet->writeInt32(base);
    packet->writeInt8(star);
    packet->writeInt32(MapId);
    packet->writeInt32(Mid);
    packet->writeInt32(time);
}