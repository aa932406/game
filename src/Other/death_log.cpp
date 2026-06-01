#include "Other/death_log.h"
#include "Other/Answer.h"

death_log::death_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

death_log::~death_log()
{
    platform_log_base::~platform_log_base(this);
}

void death_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    death_type = 0;
    death_time = 0;
    mapId = 0;
    level = 0;
    kill_id = 0;
    x = 0;
    y = 0;
}

void death_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(death_type);
    packet->writeInt32(death_time);
    packet->writeInt32(mapId);
    packet->writeInt64(kill_id);
    packet->writeInt32(level);
    packet->writeInt32(x);
    packet->writeInt32(y);
}

void death_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    death_type = inPacket->readInt32();
    death_time = inPacket->readInt32();
    mapId = inPacket->readInt32();
    kill_id = inPacket->readInt64();
    level = inPacket->readInt32();
    x = inPacket->readInt32();
    y = inPacket->readInt32();
}
