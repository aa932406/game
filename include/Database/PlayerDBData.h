#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

struct IDataStruct {};

class PlayerDBData : public IDataStruct {
public:
    PlayerDBData();
    ~PlayerDBData();
    void CleanUp();
    void SaveToSqlString(class SqlStringList *const sqls, char (*const szSQL)[4096], int64_t nCid);
    bool LoadFromDB(class Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, int64_t nCid);
    void PackageData(class Answer::NetPacket *packet);

    // 背包数据 (供 CExtCharBag 使用)
    struct BagData {
        char bagData[56 * sizeof(struct MemChrBag)];
        std::map<int32_t, int32_t> m_ItemLimit;
    } bagData;

private:
    // TODO: other member variables
};
