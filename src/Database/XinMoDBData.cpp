#include "Database/XinMoDBData.h"
#include <sstream>
#include "Answer/MySqlQuery.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/NetPacket.h"
#include "Answer/StringUtility.h"

XinMoDBData::XinMoDBData()
    : m_Exp(0)
    , m_XinMoLevel(0)
    , m_XinMoActiveLevel(0)
    , m_QiQingInfo(0)
{
    bzero(m_ItemList, sizeof(m_ItemList));
}

XinMoDBData::~XinMoDBData()
{
}

void XinMoDBData::CleanUp()
{
    m_Exp = 0;
    m_XinMoLevel = 0;
    m_XinMoActiveLevel = 0;
    m_QiQingLevel.clear();
    m_QiQingInfo = 0;
    bzero(m_ItemList, sizeof(m_ItemList));
}

void XinMoDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    for (int i = 0; i < 56; ++i)
    {
        bzero(szSQL, 0x1000u);
        snprintf(
            (char *)szSQL,
            0xFFFu,
            "INSERT INTO `mem_chr_bag` (`cid`, `bag`, `slot`, `id`, `class`, `count`, `bind`, `endtime`, `srcid`) "
            "VALUES (%lld, %d, %d, %d, %d, %d, %d, %d,%lld) "
            "on duplicate key update `id`=%d, `class`=%d, `count`=%d, `bind`=%d, `endtime`=%d, `srcid`=%lld",
            nCid,
            4,
            i,
            m_ItemList[i].itemId,
            m_ItemList[i].itemClass,
            m_ItemList[i].itemCount,
            m_ItemList[i].bind,
            m_ItemList[i].endTime,
            m_ItemList[i].srcId,
            m_ItemList[i].itemId,
            m_ItemList[i].itemClass,
            m_ItemList[i].itemCount,
            m_ItemList[i].bind,
            m_ItemList[i].endTime,
            m_ItemList[i].srcId);

        sqls->push_back(std::string((char*)szSQL));
    }

    std::string qiQingStr = GetQiQingLevelString();
    bzero(szSQL, 0x1000u);
    snprintf(
        (char *)szSQL,
        0xFFFu,
        "INSERT INTO `mem_xin_mo` (`cid`,`exp`,`xin_mo_level`,`xin_mo_act_level`,`xin_qing_info`,`qi_qing_level`) "
        "VALUES (%lld,%d,%d,%d,%d,'%s') "
        "ON DUPLICATE KEY UPDATE `exp`=%d,`xin_mo_level`=%d,`xin_mo_act_level`=%d,`xin_qing_info`=%d,`qi_qing_level`='%s'",
        nCid,
        m_Exp,
        m_XinMoLevel,
        m_XinMoActiveLevel,
        m_QiQingInfo,
        qiQingStr.c_str(),
        m_Exp,
        m_XinMoLevel,
        m_XinMoActiveLevel,
        m_QiQingInfo,
        qiQingStr.c_str());

    sqls->push_back(std::string((char*)szSQL));
}

bool XinMoDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    // Load bag items
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_chr_bag` WHERE `cid`=%lld AND `bag`=%d", nCid, 4);

    {
        Answer::MySqlQuery result(db->query((const char *)szSQL));
        while (!result.eof())
        {
            int32_t slot = result.getIntValue("slot", 0);
            if ((uint32_t)slot >= 56)
            {
                Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
                    "wrong gamble slot data slot = %d, cid = %lld\n", slot, nCid);
            }
            else
            {
                m_ItemList[slot].itemId = result.getIntValue("id", 0);
                m_ItemList[slot].itemClass = (int8_t)result.getIntValue("class", 0);
                m_ItemList[slot].itemCount = result.getIntValue("count", 0);
                m_ItemList[slot].bind = (int8_t)result.getIntValue("bind", 0);
                m_ItemList[slot].endTime = result.getIntValue("endtime", 0);
                m_ItemList[slot].srcId = result.getInt64Value("srcid", 0);
            }
            result.nextRow();
        }
    }

    // Load xin mo data
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_xin_mo` WHERE `cid`=%lld", nCid);

    {
        Answer::MySqlQuery result(db->query((const char *)szSQL));
        while (!result.eof())
        {
            m_Exp = result.getIntValue("exp", 0);
            m_XinMoLevel = result.getIntValue("xin_mo_level", 0);
            m_XinMoActiveLevel = result.getIntValue("xin_mo_act_level", 0);
            m_QiQingInfo = result.getIntValue("xin_qing_info", 0);

            std::string QiQingLevelString = result.getStringValue("qi_qing_level", "");
            if (!QiQingLevelString.empty())
            {
                std::string delims = "|";
                StringVector vTypes;
                StringUtility::split(vTypes, QiQingLevelString, delims);
                for (const auto& typeStr : vTypes)
                {
                    std::string subDelims = ":";
                    StringVector vPair;
                    StringUtility::split(vPair, typeStr, subDelims);
                    if (vPair.size() >= 2)
                    {
                        int32_t nType = atoi(vPair[0].c_str());
                        int32_t nLevel = atoi(vPair[1].c_str());
                        m_QiQingLevel[nType] = nLevel;
                    }
                }
            }
            result.nextRow();
        }
    }

    return true;
}

void XinMoDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt32(packet, m_Exp);
    Answer::NetPacket::writeInt32(packet, m_XinMoLevel);
    Answer::NetPacket::writeInt32(packet, m_XinMoActiveLevel);

    int32_t Count = 0;
    int32_t OldOffet = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::writeInt32(packet, 0);

    for (int i = 0; i < 56; ++i)
    {
        if (m_ItemList[i].itemId > 0 && m_ItemList[i].itemCount > 0)
        {
            ++Count;
            Answer::NetPacket::writeInt32(packet, i);
            Answer::NetPacket::writeInt32(packet, m_ItemList[i].itemId);
            Answer::NetPacket::writeInt8(packet, m_ItemList[i].itemClass);
            Answer::NetPacket::writeInt32(packet, m_ItemList[i].itemCount);
            Answer::NetPacket::writeInt8(packet, m_ItemList[i].bind);
            Answer::NetPacket::writeInt32(packet, m_ItemList[i].endTime);
            Answer::NetPacket::writeInt64(packet, m_ItemList[i].srcId);
        }
    }

    int32_t NewOffet = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, OldOffet);
    Answer::NetPacket::writeInt32(packet, Count);
    Answer::NetPacket::setWOffset(packet, NewOffet);

    Answer::NetPacket::writeInt32(packet, (int32_t)m_QiQingLevel.size());
    for (const auto& [type, level] : m_QiQingLevel)
    {
        Answer::NetPacket::writeInt32(packet, type);
        Answer::NetPacket::writeInt32(packet, level);
    }
    Answer::NetPacket::writeInt32(packet, m_QiQingInfo);
}

void XinMoDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{
    if (!inPacket) return;

    m_Exp = Answer::NetPacket::readInt32(inPacket);
    m_XinMoLevel = Answer::NetPacket::readInt32(inPacket);
    m_XinMoActiveLevel = Answer::NetPacket::readInt32(inPacket);

    int32_t Count = Answer::NetPacket::readInt32(inPacket);
    for (int i = 0; i < Count; ++i)
    {
        int32_t Index = Answer::NetPacket::readInt32(inPacket);
        if ((uint32_t)Index < 56)
        {
            m_ItemList[Index].itemId = Answer::NetPacket::readInt32(inPacket);
            m_ItemList[Index].itemClass = Answer::NetPacket::readInt8(inPacket);
            m_ItemList[Index].itemCount = Answer::NetPacket::readInt32(inPacket);
            m_ItemList[Index].bind = Answer::NetPacket::readInt8(inPacket);
            m_ItemList[Index].endTime = Answer::NetPacket::readInt32(inPacket);
            m_ItemList[Index].srcId = Answer::NetPacket::readInt64(inPacket);
        }
    }

    int32_t nSize = Answer::NetPacket::readInt32(inPacket);
    for (int j = 0; j < nSize; ++j)
    {
        int32_t nType = Answer::NetPacket::readInt32(inPacket);
        int32_t nLevel = Answer::NetPacket::readInt32(inPacket);
        m_QiQingLevel[nType] = nLevel;
    }

    m_QiQingInfo = Answer::NetPacket::readInt32(inPacket);
}

std::string XinMoDBData::GetQiQingLevelString()
{
    std::stringstream ss;
    bool first = true;
    for (const auto& [type, level] : m_QiQingLevel)
    {
        if (!first) ss << "|";
        ss << type << ":" << level;
        first = false;
    }
    return ss.str();
}
