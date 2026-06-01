#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class XinMoDBData : public IDataStruct {
public:
    XinMoDBData();
    ~XinMoDBData();
    void CleanUp(XinMoDBData *const this);
    void SaveToSqlString(XinMoDBData *const this, SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(XinMoDBData *const this, Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(XinMoDBData *const this, Answer::NetPacket *packet);
    void UnPackageData(XinMoDBData *const this, Answer::NetPacket *inPacket, CharId_t nCid);
    std::string GetQiQingLevelString(XinMoDBData *const this);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
