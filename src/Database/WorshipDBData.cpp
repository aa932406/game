#include "Database/WorshipDBData.h"
#include "Answer/MySqlQuery.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/NetPacket.h"

WorshipDBData::WorshipDBData()
    : nTimes(0)
{
}

WorshipDBData::~WorshipDBData()
{
}

void WorshipDBData::CleanUp()
{
    nTimes = 0;
    strCharList.clear();
}

void WorshipDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    bzero(szSQL, 0x1000u);
    snprintf(
        (char *)szSQL,
        0xFFFu,
        "INSERT INTO `mem_char_worship` (cid,times,charlist) VALUES (%lld,%d,'%s') on duplicate key update times=%d,charlist='%s'",
        nCid,
        nTimes,
        strCharList.c_str(),
        nTimes,
        strCharList.c_str());

    sqls->push_back(std::string((char*)szSQL));
}

bool WorshipDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_worship` WHERE `cid`=%lld", nCid);

    Answer::MySqlQuery result(db->query((const char *)szSQL));
    if (!result.eof())
    {
        nTimes = result.getIntValue("times", 0);
        strCharList = result.getStringValue("charlist", "");
        return true;
    }
    return false;
}

void WorshipDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt32(packet, nTimes);
    Answer::NetPacket::writeUTF8(packet, &strCharList);
}
