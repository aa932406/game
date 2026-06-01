#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class MagicBoxDBData : public IDataStruct {
public:
    MagicBoxDBData();
    ~MagicBoxDBData();
    void CleanUp(MagicBoxDBData *const this);
    void SaveToSqlString(MagicBoxDBData *const this, SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(MagicBoxDBData *const this, Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(MagicBoxDBData *const this, Answer::NetPacket *packet);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
