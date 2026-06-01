#include "Other/proc_log.h"
#include "Other/Answer.h"

proc_log::proc_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

proc_log::~proc_log()
{
    platform_log_base::~platform_log_base(this);
}

void proc_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    op_time = 0;
    proc_id = 0;
}

void proc_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(op_time);
    packet->writeInt32(proc_id);
}

void proc_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    op_time = inPacket->readInt32();
    proc_id = inPacket->readInt32();
}
