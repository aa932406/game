#include "Database/CurrencyDBData.h"
#include "Answer/NetPacket.h"
#include <cstdio>
#include <cstring>

CurrencyDBData::CurrencyDBData()
{
    CleanUp();
}

CurrencyDBData::~CurrencyDBData()
{
}

void CurrencyDBData::CleanUp()
{
    memset(vCurrency, 0, sizeof(vCurrency));
}

void CurrencyDBData::SaveToSqlString(SqlStringList *sqls, char (*szSQL)[4096], CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)sqls;
    (void)szSQL;
    (void)nCid;
}

bool CurrencyDBData::LoadFromDB(Answer::MySqlDBGuard *db, char (*szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    // TODO: implement from decompiled code
    (void)db;
    (void)szSQL;
    (void)nUid;
    (void)nSid;
    (void)nCid;
    return false;
}

void CurrencyDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    for (int32_t i = 0; i < 12; ++i)
        Answer::NetPacket::writeInt64(packet, vCurrency[i]);
}
