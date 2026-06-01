#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class WuHunShopDBData : public IDataStruct {
public:
    WuHunShopDBData();
    ~WuHunShopDBData();
    void CleanUp(WuHunShopDBData *const this);
    void SaveToSqlString(WuHunShopDBData *const this, SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(WuHunShopDBData *const this, Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(WuHunShopDBData *const this, Answer::NetPacket *packet);
    void UnPackageData(WuHunShopDBData *const this, Answer::NetPacket *inPacket, CharId_t nCid);
    void ParesWuHun(WuHunShopDBData *const this, std::string *p_WuHunString);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
