#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Common/CommonTypes.h"
#include "Character/CExtCharMysteryShop.h"

class MysteryShopDBData : public IDataStruct {
public:
    MysteryShopDBData();
    ~MysteryShopDBData();
    void SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket *packet);

    std::list<MysteryShop> lstShop;
};
