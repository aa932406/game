#include "Other/currency_log.h"
#include "Other/Answer.h"

currency_log::currency_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

currency_log::~currency_log()
{
    platform_log_base::~platform_log_base(this);
}

void currency_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    time = 0;
    type = 0;
    opway = 0;
    value = 0;
    ids.clear();
    couts.clear();
    level = 0;
}

void currency_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(time);
    packet->writeInt8(type);
    packet->writeInt32(opway);
    packet->writeInt64(value);
    packet->writeUTF8(ids);
    packet->writeUTF8(couts);
    packet->writeInt32(level);
}

void currency_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    time = inPacket->readInt32();
    type = inPacket->readInt8();
    opway = inPacket->readInt32();
    value = inPacket->readInt64();
    inPacket->readUTF8(ids);
    inPacket->readUTF8(couts);
    level = inPacket->readInt32();
}
