#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class MailDBData : public IDataStruct {
public:
    MailDBData();
    ~MailDBData();
    void SaveToSqlString(MailDBData *const this, SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(MailDBData *const this, Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(MailDBData *const this, Answer::NetPacket *packet);
    void UnPackageData(MailDBData *const this, Answer::NetPacket *inPacket, CharId_t nCid);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
