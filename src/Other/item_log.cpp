#include "Other/item_log.h"
#include "Other/Answer.h"

item_log::item_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

item_log::~item_log()
{
    platform_log_base::~platform_log_base(this);
}

void item_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    op_time = 0;
    ItemId.clear();
    flag = 0;
    opway = 0;
    count = 0;
    level = 0;
}

void item_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(op_time);
    packet->writeUTF8(ItemId);
    packet->writeInt32(flag);
    packet->writeInt32(opway);
    packet->writeInt32(count);
    packet->writeInt32(level);
}

void item_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    op_time = inPacket->readInt32();
    inPacket->readUTF8(ItemId);
    flag = inPacket->readInt32();
    opway = inPacket->readInt32();
    count = inPacket->readInt32();
    level = inPacket->readInt32();
}
