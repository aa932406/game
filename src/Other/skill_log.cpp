#include "Other/skill_log.h"
#include "Other/Answer.h"

skill_log::skill_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

skill_log::~skill_log()
{
    platform_log_base::~platform_log_base(this);
}

void skill_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    op_time = 0;
    skill_id = 0;
    oper_id = 0;
    level = 0;
}

void skill_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(op_time);
    packet->writeInt32(skill_id);
    packet->writeInt32(oper_id);
    packet->writeInt32(level);
}

void skill_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    op_time = inPacket->readInt32();
    skill_id = inPacket->readInt32();
    oper_id = inPacket->readInt32();
    level = inPacket->readInt32();
}
