#include "Other/bossjoin_log.h"
#include "Network/NetPacket.h"

bossjoin_log::bossjoin_log()
{
    CleanUp();
}

bossjoin_log::~bossjoin_log()
{
}

void bossjoin_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport = "";
    cid = 0;
    name = "";
    join_time = 0;
    mid = 0;
    boss_id = 0;
    mapId = 0;
    killed = 0;
}

void bossjoin_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(&passport);
    packet->writeInt64(cid);
    packet->writeUTF8(&name);
    packet->writeInt32(join_time);
    packet->writeInt32(mid);
    packet->writeInt32(boss_id);
    packet->writeInt32(mapId);
    packet->writeInt32(killed);
}

void bossjoin_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(&passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(&name);
    join_time = inPacket->readInt32();
    mid = inPacket->readInt32();
    boss_id = inPacket->readInt32();
    mapId = inPacket->readInt32();
    killed = inPacket->readInt32();
}
