#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Database/PlayerDBData.h"
#include "Common/CommonTypes.h"

class XinMoDBData : public IDataStruct {
public:
    XinMoDBData();
    ~XinMoDBData();
    void CleanUp();
    void SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid);
    std::string GetQiQingLevelString();

    int32_t m_Exp;
    int32_t m_XinMoLevel;
    int32_t m_XinMoActiveLevel;
    std::map<int, int> m_QiQingLevel;
    int32_t m_QiQingInfo;
    MemChrBag m_ItemList[56];
};
