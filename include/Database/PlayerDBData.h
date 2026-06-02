#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Common/CommonTypes.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtCharExchange.h"
#include "Other/RwLock.h"

namespace Answer { class MySqlDBGuard; }



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
        MemChrBag bagData[56];
        std::map<int32_t, int32_t> m_ItemLimit;
    } bagData;

    // 仓库数据 (供 CExtCharDepot 使用)
    struct GambleDepot {
        int32_t OpneCount;
        std::string Password;
        std::string SecondPassword;
        MemChrBag gambleDepot[560];
        int64_t Currency[12];
    } gambleDepot;

    // 交换数据 (供 CExtCharExchange 使用)
    struct {
        std::list<ExchangeRecord> lstExchange;
    } m_ExchangeDBData;

private:
    // TODO: other member variables
};
