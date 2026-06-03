#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Common/CommonTypes.h"

struct WuHunShop
{
    int32_t nIndex[4];
    int8_t nState[4];
};

class WuHunShopDBData : public IDataStruct {
public:
    WuHunShopDBData();
    ~WuHunShopDBData();
    void CleanUp();
    void SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid);
    void ParesWuHun(std::string *p_WuHunString);

    std::map<int, WuHunShop> m_WuHunShopMap;
    int32_t m_RefreshTimes;
    int32_t m_LastRefreshTime;
    int32_t m_WuHun[5][16];
};
