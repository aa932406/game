#include "Other/SysUserData.h"
#include "Network/NetPacket.h"
#include <cstring>
#include <cstdio>

SysUserData::SysUserData()
{
    CleanUp();
}

SysUserData::~SysUserData()
{
}

void SysUserData::CleanUp()
{
    memset(&data, 0, sizeof(data));
}

void SysUserData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt64(data.uid);
    packet->writeInt32(data.sid);
    packet->writeInt32(data.gold_pay);
    packet->writeInt32(data.gold);
    packet->writeInt32(data.sys_gold);
    packet->writeInt32(data.gold_pay_total);
    packet->writeInt32(data.prepay_gold);
    packet->writeInt32(data.gold_cost_total);
    packet->writeInt32(data.cash);
    packet->writeInt32(data.map_enter_time);
    packet->writeInt32(data.last_login_time);
    packet->writeInt8(data.adult);
    packet->writeInt8(data.gm_level);
    packet->writeInt32(data.last_logout_time);
    packet->writeInt32(data.continue_login_count);
    packet->writeInt32(data.total_online_time);
    packet->writeInt32(data.total_offline_time);
    packet->writeUTF8(data.IP);
    packet->writeUTF8(data.passport);
    packet->writeUTF8(data.platform);
    packet->writeInt8(data.fuli);
    packet->writeInt8(data.robot);
    packet->writeInt8(data.mini_client);
    packet->writeInt8(data.net_bar);
    packet->writeUTF8(data.from_way);
    packet->writeInt8(data.JianKong);
}

void SysUserData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    data.uid = inPacket->readInt64();
    data.sid = inPacket->readInt32();
    data.gold_pay = inPacket->readInt32();
    data.gold = inPacket->readInt32();
    data.sys_gold = inPacket->readInt32();
    data.gold_pay_total = inPacket->readInt32();
    data.prepay_gold = inPacket->readInt32();
    data.gold_cost_total = inPacket->readInt32();
    data.cash = inPacket->readInt32();
    data.map_enter_time = inPacket->readInt32();
    data.last_login_time = inPacket->readInt32();
    data.adult = inPacket->readInt8();
    data.gm_level = inPacket->readInt8();
    data.last_logout_time = inPacket->readInt32();
    data.continue_login_count = inPacket->readInt32();
    data.total_online_time = inPacket->readInt32();
    data.total_offline_time = inPacket->readInt32();

    std::string tmp;
    inPacket->readUTF8(tmp);
    snprintf(data.IP, sizeof(data.IP), "%s", tmp.c_str());
    inPacket->readUTF8(tmp);
    snprintf(data.passport, sizeof(data.passport), "%s", tmp.c_str());
    inPacket->readUTF8(tmp);
    snprintf(data.platform, sizeof(data.platform), "%s", tmp.c_str());

    data.fuli = inPacket->readInt8();
    data.robot = inPacket->readInt8();
    data.mini_client = inPacket->readInt8();
    data.net_bar = inPacket->readInt8();

    inPacket->readUTF8(tmp);
    snprintf(data.from_way, sizeof(data.from_way), "%s", tmp.c_str());

    data.JianKong = inPacket->readInt8();
}