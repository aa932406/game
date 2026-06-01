#include "Other/important_system_log.h"
#include "Other/Answer.h"

important_system_log::important_system_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

important_system_log::~important_system_log()
{
    platform_log_base::~platform_log_base(this);
}

void important_system_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    up_time = 0;
    sys_name = 0;
    oper_id = 0;
    level = 0;
}

void important_system_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(up_time);
    packet->writeInt32(sys_name);
    packet->writeInt32(oper_id);
    packet->writeInt32(level);
}

void important_system_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    up_time = inPacket->readInt32();
    sys_name = inPacket->readInt32();
    oper_id = inPacket->readInt32();
    level = inPacket->readInt32();
}
