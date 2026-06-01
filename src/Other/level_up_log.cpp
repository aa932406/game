#include "Other/level_up_log.h"
#include "Other/Answer.h"

level_up_log::level_up_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

level_up_log::~level_up_log()
{
    platform_log_base::~platform_log_base(this);
}

void level_up_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    create_time = 0;
    level_up_time = 0;
    level = 0;
    Level_interval = 0;
}

void level_up_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(create_time);
    packet->writeInt32(level_up_time);
    packet->writeInt32(level);
    packet->writeInt32(Level_interval);
}

void level_up_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    create_time = inPacket->readInt32();
    level_up_time = inPacket->readInt32();
    level = inPacket->readInt32();
    Level_interval = inPacket->readInt32();
}
