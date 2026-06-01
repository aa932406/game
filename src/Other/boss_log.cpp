#include "Other/boss_log.h"
#include "Network/NetPacket.h"

boss_log::boss_log()
{
    CleanUp();
}

boss_log::~boss_log()
{
}

void boss_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport = "";
    cid = 0;
    name = "";
    kill_time = 0;
    mid = 0;
    boss_id = 0;
}

void boss_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(&passport);
    packet->writeInt64(cid);
    packet->writeUTF8(&name);
    packet->writeInt32(kill_time);
    packet->writeInt32(mid);
    packet->writeInt32(boss_id);
}

void boss_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(&passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(&name);
    kill_time = inPacket->readInt32();
    mid = inPacket->readInt32();
    boss_id = inPacket->readInt32();
}
