#include "Database/ExchangeDBData.h"
#include "Answer/DayTime.h"

ExchangeDBData::ExchangeDBData() {
}

ExchangeDBData::~ExchangeDBData() {
}

void ExchangeDBData::SaveToSqlString(SqlStringList* const sqls, char (*const szSQL)[4096], CharId_t nCid) {
    bzero(szSQL, 0x1000u);
    snprintf((char*)szSQL, 0xFFFu, "DELETE FROM `mem_char_exchange` WHERE `cid`=%lld", nCid);
    sqls->push_back(std::string((char*)szSQL));

    for (const auto& record : lstExchange) {
        bzero(szSQL, 0x1000u);
        snprintf((char*)szSQL, 0xFFFu,
            "INSERT INTO `mem_char_exchange` (`cid`,`type`,`index`,`count`) VALUES (%lld,%d,%d,%d)",
            nCid, record.nType, record.nIndex, record.nCount);
        sqls->push_back(std::string((char*)szSQL));
    }
}

bool ExchangeDBData::LoadFromDB(Answer::MySqlDBGuard* const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid) {
    (void)nUid;
    (void)nSid;
    bzero(szSQL, 0x1000u);
    snprintf((char*)szSQL, 0xFFFu, "SELECT * FROM `mem_char_exchange` WHERE `cid`=%lld", nCid);
    Answer::MySqlQuery result(db->query((const char*)szSQL));
    while (!result.eof()) {
        ExchangeRecord shop = {};
        shop.nType = static_cast<int8_t>(result.getIntValue("type", 0));
        shop.nIndex = static_cast<int16_t>(result.getIntValue("index", 0));
        shop.nCount = result.getIntValue("count", 0);
        lstExchange.push_back(shop);
        result.nextRow();
    }
    return false;
}

void ExchangeDBData::PackageData(Answer::NetPacket* packet) {
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, static_cast<int32_t>(lstExchange.size()));
    for (const auto& record : lstExchange) {
        Answer::NetPacket::writeInt8(packet, record.nType);
        Answer::NetPacket::writeInt16(packet, record.nIndex);
        Answer::NetPacket::writeInt32(packet, record.nCount);
    }
}
