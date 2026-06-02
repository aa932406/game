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
#include "Mem/MemChrBag.h"
#include "Other/MailInfo.h"

class MailDBData {
public:
    MailDBData();
    ~MailDBData();
    void SaveToSqlString(SqlStringList* const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard* const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket* packet);
    void UnPackageData(Answer::NetPacket* inPacket, CharId_t nCid);
    static void ParesChrBagString(MemChrBag* out, MailDBData* _this, std::string* p_ItemString);

private:
    std::map<int, MailInfo> m_MailInfo;
};
