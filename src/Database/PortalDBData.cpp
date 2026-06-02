#include "Database/PortalDBData.h"

PortalDBData::PortalDBData()
    : nPortalId(0)
{
}

PortalDBData::~PortalDBData()
{
}

void PortalDBData::CleanUp()
{
    lstPortal.clear();
    nPortalId = 0;
}

void PortalDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    for (const auto& info : lstPortal)
    {
        bzero(szSQL, 0x1000u);
        snprintf(
            (char *)szSQL,
            0xFFFu,
            "INSERT INTO `mem_char_portal` (`cid`,`id`,`map`,`x`,`y`,`dungeon`,`time`,`duration`,`close`) VALUES (%lld,%d,%d,%d,%d,%d,%d,%d,%d) "
            "ON DUPLICATE KEY UPDATE `map`=%d,`x`=%d,`y`=%d,`dungeon`=%d,`time`=%d,`duration`=%d,`close`=%d",
            nCid,
            info.nId,
            info.nMapId,
            info.nPosX,
            info.nPosY,
            info.nDungeon,
            info.nStartTime,
            info.nDuration,
            info.bClose,
            info.nMapId,
            info.nPosX,
            info.nPosY,
            info.nDungeon,
            info.nStartTime,
            info.nDuration,
            info.bClose);

        sqls->push_back(std::string((char*)szSQL));
    }
}

bool PortalDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    bzero(szSQL, 0x1000u);
    snprintf(
        (char *)szSQL,
        0xFFFu,
        "SELECT * FROM `mem_char_portal` WHERE `cid`=%lld AND `close`=0 ORDER BY `time`",
        nCid);

    Answer::MySqlQuery result(db->query((const char *)szSQL));
    while (!result.eof())
    {
        PortalInfo info;
        memset(&info, 0, sizeof(info));
        info.nId = result.getIntValue("id", 0);
        info.nMapId = result.getIntValue("map", 0);
        info.nPosX = result.getIntValue("x", 0);
        info.nPosY = result.getIntValue("y", 0);
        info.nDungeon = result.getIntValue("dungeon", 0);
        info.nStartTime = result.getIntValue("time", 0);
        info.nDuration = result.getIntValue("duration", 0);
        lstPortal.push_back(info);
        nPortalId = info.nId;
        result.nextRow();
    }
    return true;
}

void PortalDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt32(packet, (int32_t)lstPortal.size());
    for (const auto& info : lstPortal)
    {
        Answer::NetPacket::writeInt32(packet, info.nId);
        Answer::NetPacket::writeInt32(packet, info.nMapId);
        Answer::NetPacket::writeInt32(packet, info.nPosX);
        Answer::NetPacket::writeInt32(packet, info.nPosY);
        Answer::NetPacket::writeInt32(packet, info.nDungeon);
        Answer::NetPacket::writeInt32(packet, info.nStartTime);
        Answer::NetPacket::writeInt32(packet, info.nDuration);
        Answer::NetPacket::writeInt8(packet, info.bClose);
    }
    Answer::NetPacket::writeInt32(packet, nPortalId);
}
