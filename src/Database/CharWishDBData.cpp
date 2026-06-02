#include "Database/CharWishDBData.h"
#include "Answer/NetPacket.h"
#include <cstdio>
#include <cstring>

CharWishDBData::CharWishDBData()
{
}

CharWishDBData::~CharWishDBData()
{
}

void CharWishDBData::SaveToSqlString(SqlStringList *sqls, char (*szSQL)[4096], CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)sqls;
    (void)szSQL;
    (void)nCid;
}

bool CharWishDBData::LoadFromDB(Answer::MySqlDBGuard *db, char (*szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)db;
    (void)szSQL;
    (void)nUid;
    (void)nSid;
    (void)nCid;
    return false;
}

void CharWishDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    Answer::NetPacket::writeInt32(packet, static_cast<int32_t>(lstWishs.size()));
    for (const auto& wish : lstWishs)
    {
        Answer::NetPacket::writeInt32(packet, wish.nId);
        Answer::NetPacket::writeInt32(packet, wish.nItemId);
        Answer::NetPacket::writeInt32(packet, wish.nStartTime);
        Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(wish.nGetReward));
        Answer::NetPacket::writeUTF8(packet, &wish.strInfo);
    }
}
