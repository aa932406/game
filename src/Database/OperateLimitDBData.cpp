#include "Database/OperateLimitDBData.h"
#include "Answer/MySqlQuery.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/NetPacket.h"

OperateLimitDBData::OperateLimitDBData()
{
}

OperateLimitDBData::~OperateLimitDBData()
{
}

void OperateLimitDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    for (const auto& pair : m_mOperateLimit)
    {
        const auto& limit = pair.second;
        bzero(szSQL, 0x1000u);
        snprintf(
            (char *)szSQL,
            0xFFFu,
            "INSERT `mem_char_operate_limit`(cid,LimitId,LimitCount) VALUES (%lld,%d,%d) ON DUPLICATE KEY UPDATE LimitCount=%d",
            nCid,
            limit.LimitId,
            limit.LimitCount,
            limit.LimitCount);

        sqls->push_back(std::string((char*)szSQL));
    }
}

bool OperateLimitDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_operate_limit` WHERE `cid`=%lld", nCid);

    Answer::MySqlQuery result(db->query((const char *)szSQL));
    while (!result.eof())
    {
        OperateLimit limit;
        limit.LimitId = result.getIntValue("LimitId", 0);
        limit.LimitCount = result.getIntValue("LimitCount", 0);
        m_mOperateLimit[limit.LimitId] = limit;
        result.nextRow();
    }
    return true;
}

void OperateLimitDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt32(packet, (int32_t)m_mOperateLimit.size());
    for (const auto& pair : m_mOperateLimit)
    {
        const auto& limit = pair.second;
        Answer::NetPacket::writeInt32(packet, limit.LimitId);
        Answer::NetPacket::writeInt32(packet, limit.LimitCount);
    }
}
