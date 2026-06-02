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

class MagicBoxDBData {
public:
    MagicBoxDBData();
    ~MagicBoxDBData();
    void CleanUp();
    void SaveToSqlString(SqlStringList* const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard* const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket* packet);

private:
    int32_t nCombiPoints;
    int32_t nLastReviveTime;
    std::string strActiveScrolls;
    std::string strSuccessIds;
};
