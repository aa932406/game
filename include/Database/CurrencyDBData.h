#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

namespace Answer {
    class MySqlDBGuard;
    class MySqlQuery;
    class NetPacket;
}

typedef int64_t CharId_t;
typedef int64_t FamilyId_t;
typedef std::list<std::string> SqlStringList;

class CurrencyDBData {
public:
    CurrencyDBData();
    ~CurrencyDBData();
    void CleanUp();
    void SaveToSqlString(SqlStringList *sqls, char (*szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard *db, char (*szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket *packet);

public:
    int64_t vCurrency[12];
};
