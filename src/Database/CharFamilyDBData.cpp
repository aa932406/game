#include "Database/CharFamilyDBData.h"
#include "Answer/NetPacket.h"
#include <cstdio>
#include <cstring>

CharFamilyDBData::CharFamilyDBData()
    : nFamilyId(0)
    , nPosition(0)
    , nContribution(0)
    , nDailyContribution(0)
    , nMedLevel(0)
    , nMedRes(0)
    , nHoe(0)
    , EnterDungeionFamilyId(0)
    , EnterCount(0)
    , EnterTime(0)
{
    CleanUp();
}

CharFamilyDBData::~CharFamilyDBData()
{
}

void CharFamilyDBData::CleanUp()
{
    nFamilyId = 0;
    nPosition = 0;
    nContribution = 0;
    nDailyContribution = 0;
    strSkills.clear();
    nMedLevel = 0;
    nMedRes = 0;
    nHoe = 0;
    EnterDungeionFamilyId = 0;
    EnterCount = 0;
    EnterTime = 0;
}

void CharFamilyDBData::SaveToSqlString(SqlStringList *sqls, char (*szSQL)[4096], CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)sqls;
    (void)szSQL;
    (void)nCid;
}

bool CharFamilyDBData::LoadFromDB(Answer::MySqlDBGuard *db, char (*szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)db;
    (void)szSQL;
    (void)nUid;
    (void)nSid;
    (void)nCid;
    return false;
}

void CharFamilyDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    Answer::NetPacket::writeInt64(packet, nFamilyId);
    Answer::NetPacket::writeInt8(packet, nPosition);
    Answer::NetPacket::writeInt32(packet, nContribution);
    Answer::NetPacket::writeInt32(packet, nDailyContribution);
    Answer::NetPacket::writeUTF8(packet, &strSkills);
    Answer::NetPacket::writeInt32(packet, nMedLevel);
    Answer::NetPacket::writeInt32(packet, nMedRes);
    Answer::NetPacket::writeInt32(packet, nHoe);
    Answer::NetPacket::writeInt64(packet, EnterDungeionFamilyId);
    Answer::NetPacket::writeInt32(packet, EnterCount);
    Answer::NetPacket::writeInt32(packet, EnterTime);
}
