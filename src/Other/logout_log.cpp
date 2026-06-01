#include "Other/logout_log.h"
#include "Other/Answer.h"

logout_log::logout_log()
{
    platform_log_base::platform_log_base(this);
    CleanUp();
}

logout_log::~logout_log()
{
    platform_log_base::~platform_log_base(this);
}

void logout_log::CleanUp()
{
    platform_log_base::CleanUp();
    passport.clear();
    cid = 0;
    name.clear();
    create_time = 0;
    logout_time = 0;
    online_time = 0;
    ip.clear();
    job = 0;
    level = 0;
    battle = 0;
    huoyuedu = 0;
    map_id = 0;
    task_id = 0;
    recharge_sum = 0;
    gold = 0;
    cash = 0;
    money = 0;
    bind_money = 0;
}

void logout_log::PackageData(Answer::NetPacket *packet)
{
    platform_log_base::PackageData(packet);
    packet->writeUTF8(passport);
    packet->writeInt64(cid);
    packet->writeUTF8(name);
    packet->writeInt32(create_time);
    packet->writeInt32(logout_time);
    packet->writeInt32(online_time);
    packet->writeUTF8(ip);
    packet->writeInt8(job);
    packet->writeInt32(level);
    packet->writeInt64(battle);
    packet->writeInt32(huoyuedu);
    packet->writeInt32(map_id);
    packet->writeInt32(task_id);
    packet->writeInt64(recharge_sum);
    packet->writeInt64(gold);
    packet->writeInt64(cash);
    packet->writeInt64(money);
    packet->writeInt64(bind_money);
}

void logout_log::UnPackageData(Answer::NetPacket *inPacket)
{
    platform_log_base::UnPackageData(inPacket);
    inPacket->readUTF8(passport);
    cid = inPacket->readInt64();
    inPacket->readUTF8(name);
    create_time = inPacket->readInt32();
    logout_time = inPacket->readInt32();
    online_time = inPacket->readInt32();
    inPacket->readUTF8(ip);
    job = inPacket->readInt8();
    level = inPacket->readInt32();
    battle = inPacket->readInt64();
    huoyuedu = inPacket->readInt32();
    map_id = inPacket->readInt32();
    task_id = inPacket->readInt32();
    recharge_sum = inPacket->readInt64();
    gold = inPacket->readInt64();
    cash = inPacket->readInt64();
    money = inPacket->readInt64();
    bind_money = inPacket->readInt64();
}
