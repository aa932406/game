#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Answer/NetPacket.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/MySqlQuery.h"
#include "Common/CommonTypes.h"

#ifndef EXCHANGE_RECORD_DEFINED
#define EXCHANGE_RECORD_DEFINED
struct ExchangeRecord
{
    int8_t nType;
    int16_t nIndex;
    int32_t nCount;
};
#endif

class ExchangeDBData {
public:
    ExchangeDBData();
    ~ExchangeDBData();
    void SaveToSqlString(SqlStringList* const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard* const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket* packet);

public:
    std::list<ExchangeRecord> lstExchange;
};
