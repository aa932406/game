#include "Database/CharWingDBData.h"
#include "Answer/NetPacket.h"
#include <cstdio>
#include <cstring>

CharWingDBData::CharWingDBData()
{
    CleanUp();
}

CharWingDBData::~CharWingDBData()
{
}

void CharWingDBData::CleanUp()
{
    m_Level = 0;
    m_Luck = 0;
    m_HuanHua = 0;
}

void CharWingDBData::SaveToSqlString(SqlStringList *sqls, char (*szSQL)[4096], CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)sqls;
    (void)szSQL;
    (void)nCid;
}

bool CharWingDBData::LoadFromDB(Answer::MySqlDBGuard *db, char (*szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)db;
    (void)szSQL;
    (void)nUid;
    (void)nSid;
    (void)nCid;
    return false;
}

void CharWingDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet)
        return;
    Answer::NetPacket::writeInt32(packet, m_Level);
    Answer::NetPacket::writeInt32(packet, m_Luck);
    Answer::NetPacket::writeInt32(packet, m_HuanHua);
}

void CharWingDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{
    if (!inPacket)
        return;
    m_Level = Answer::NetPacket::readInt32(inPacket);
    m_Luck = Answer::NetPacket::readInt32(inPacket);
    m_HuanHua = Answer::NetPacket::readInt32(inPacket);
    (void)nCid;
}
