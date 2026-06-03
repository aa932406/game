#include "common.h"
#include "Other/MemCharacterData.h"
#include "Network/NetPacket.h"
#include <cstring>
#include <cstdio>

MemCharacterData::MemCharacterData()
{
    CleanUp();
}

MemCharacterData::~MemCharacterData()
{
}

void MemCharacterData::CleanUp()
{
    weapon = 0;
    cloth = 0;
    wing = 0;
    equipStar = 0;
    equipGem = 0;
    moneyDropRatio = 0;
    memset(&data, 0, sizeof(data));
}

void MemCharacterData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt64(data.cid);
    packet->writeUTF8(data.name);
    packet->writeUTF8(data.familyName);
    packet->writeInt8(data.sex);
    packet->writeInt8(data.job);
    packet->writeInt32(data.level);
    packet->writeInt32(data.battle);
    packet->writeInt32(data.top_battle);
    packet->writeInt64(data.exp);
    packet->writeInt32(data.mapid);
    packet->writeInt32(data.x);
    packet->writeInt32(data.y);
    packet->writeInt32(data.hp);
    packet->writeInt32(data.mp);
    packet->writeInt32(data.kun_li);
    packet->writeInt32(data.head);
    packet->writeInt32(data.pk_mode);
    packet->writeInt32(data.pk_value);
    packet->writeInt32(data.pk_time);
    packet->writeInt32(data.kill_count);
    packet->writeInt32(data.last_task_id);
    packet->writeInt32(data.logout_count);
    packet->writeInt32(data.pay_click_count);
    packet->writeInt32(data.level_stay_time);
    packet->writeInt32(data.last_login_time);
    packet->writeInt32(data.last_logout_time);
    packet->writeInt32(data.create_time);
    packet->writeInt32(weapon);
    packet->writeInt32(cloth);
    packet->writeInt32(wing);
    packet->writeInt32(equipStar);
    packet->writeInt32(equipGem);
    packet->writeInt32(moneyDropRatio);
}

void MemCharacterData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    data.cid = inPacket->readInt64();
    std::string tmp;
    inPacket->readUTF8(tmp);
    snprintf(data.name, sizeof(data.name), "%s", tmp.c_str());
    inPacket->readUTF8(tmp);
    snprintf(data.familyName, sizeof(data.familyName), "%s", tmp.c_str());
    data.sex = inPacket->readInt8();
    data.job = inPacket->readInt8();
    data.level = inPacket->readInt32();
    data.battle = inPacket->readInt32();
    data.top_battle = inPacket->readInt32();
    data.exp = inPacket->readInt64();
    data.mapid = inPacket->readInt32();
    data.x = inPacket->readInt32();
    data.y = inPacket->readInt32();
    data.hp = inPacket->readInt32();
    data.mp = inPacket->readInt32();
    data.kun_li = inPacket->readInt32();
    data.head = inPacket->readInt32();
    data.pk_mode = inPacket->readInt32();
    data.pk_value = inPacket->readInt32();
    data.pk_time = inPacket->readInt32();
    data.kill_count = inPacket->readInt32();
    data.last_task_id = inPacket->readInt32();
    data.logout_count = inPacket->readInt32();
    data.pay_click_count = inPacket->readInt32();
    data.level_stay_time = inPacket->readInt32();
    data.last_login_time = inPacket->readInt32();
    data.last_logout_time = inPacket->readInt32();
    data.create_time = inPacket->readInt32();
    weapon = inPacket->readInt32();
    cloth = inPacket->readInt32();
    wing = inPacket->readInt32();
    equipStar = inPacket->readInt32();
    equipGem = inPacket->readInt32();
    moneyDropRatio = inPacket->readInt32();
}