#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class CurrencyDBData : public IDataStruct {
public:
    CurrencyDBData();
    ~CurrencyDBData();
    void CleanUp(CurrencyDBData *const this);
    void SaveToSqlString(CurrencyDBData *const this, SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(CurrencyDBData *const this, Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(CurrencyDBData *const this, Answer::NetPacket *packet);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
