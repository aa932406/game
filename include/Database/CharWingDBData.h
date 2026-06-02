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
typedef std::list<std::string> SqlStringList;

class CharWingDBData {
public:
    CharWingDBData();
    ~CharWingDBData();
    void CleanUp();
    void SaveToSqlString(SqlStringList *sqls, char (*szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard *db, char (*szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid);

public:
    int32_t m_Level;
    int32_t m_Luck;
    int32_t m_HuanHua;
};
