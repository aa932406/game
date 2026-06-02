#include "Database/MagicBoxDBData.h"
#include "Answer/DayTime.h"

MagicBoxDBData::MagicBoxDBData() {
}

MagicBoxDBData::~MagicBoxDBData() {
}

void MagicBoxDBData::CleanUp() {
    nCombiPoints = 0;
    nLastReviveTime = 0;
    strActiveScrolls.clear();
    strSuccessIds.clear();
}

void MagicBoxDBData::SaveToSqlString(SqlStringList* const sqls, char (*const szSQL)[4096], CharId_t nCid) {
    bzero(szSQL, 0x1000u);
    const char* pActiveScrolls = strActiveScrolls.c_str();
    const char* pSuccessIds = strSuccessIds.c_str();
    snprintf((char*)szSQL, 0xFFFu,
        "INSERT INTO `mem_char_magic_box` (`cid`,`combi_points`,`revive_time`,`active_scrolls`,`success_ids`) "
        "VALUES (%lld,%d,%d,'%s','%s') ON DUPLICATE KEY UPDATE "
        "`combi_points`=%d,`revive_time`=%d,`active_scrolls`='%s',`success_ids`='%s'",
        nCid, nCombiPoints, nLastReviveTime, pActiveScrolls, pSuccessIds,
        nCombiPoints, nLastReviveTime, pActiveScrolls, pSuccessIds);
    sqls->push_back(std::string((char*)szSQL));
}

bool MagicBoxDBData::LoadFromDB(Answer::MySqlDBGuard* const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid) {
    (void)nUid;
    (void)nSid;
    bzero(szSQL, 0x1000u);
    snprintf((char*)szSQL, 0xFFFu, "SELECT * FROM `mem_char_magic_box` WHERE `cid`= %lld", nCid);
    Answer::MySqlQuery result(db->query((const char*)szSQL));
    if (!result.eof()) {
        nCombiPoints = result.getIntValue("combi_points", 0);
        nLastReviveTime = result.getIntValue("revive_time", 0);
        std::string StringValue = result.getStringValue("active_scrolls", "");
        strActiveScrolls = StringValue;
        std::string v8 = result.getStringValue("success_ids", "");
        strSuccessIds = v8;
    }
    return true;
}

void MagicBoxDBData::PackageData(Answer::NetPacket* packet) {
    Answer::NetPacket::writeInt32(packet, nCombiPoints);
    Answer::NetPacket::writeInt32(packet, nLastReviveTime);
    Answer::NetPacket::writeUTF8(packet, &strActiveScrolls);
    Answer::NetPacket::writeUTF8(packet, &strSuccessIds);
}
