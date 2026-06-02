#include "Database/WuHunShopDBData.h"
#include <sstream>
#include "Answer/MySqlQuery.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/NetPacket.h"
#include "Answer/StringUtility.h"

WuHunShopDBData::WuHunShopDBData()
    : m_RefreshTimes(5)
    , m_LastRefreshTime(0)
{
    bzero(m_WuHun, sizeof(m_WuHun));
}

WuHunShopDBData::~WuHunShopDBData()
{
}

void WuHunShopDBData::CleanUp()
{
    m_WuHunShopMap.clear();
    m_RefreshTimes = 5;
    m_LastRefreshTime = 0;
    bzero(m_WuHun, sizeof(m_WuHun));
}

void WuHunShopDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    for (const auto& [shopId, shop] : m_WuHunShopMap)
    {
        std::stringstream ssIndex;
        std::stringstream ssState;
        for (int i = 0; i < 4; ++i)
        {
            if (i > 0)
            {
                ssIndex << ":";
                ssState << ":";
            }
            ssIndex << shop.nIndex[i];
            ssState << (int)shop.nState[i];
        }

        bzero(szSQL, 0x1000u);
        snprintf(
            (char *)szSQL,
            0xFFFu,
            "INSERT INTO `mem_wu_hun_shop` (`cid`,`shopid`,`item_index`,`item_state`) VALUES (%lld,%d,'%s','%s') "
            "ON DUPLICATE KEY UPDATE `item_index`='%s',`item_state`='%s'",
            nCid,
            shopId,
            ssIndex.str().c_str(),
            ssState.str().c_str(),
            ssIndex.str().c_str(),
            ssState.str().c_str());

        sqls->push_back(std::string((char*)szSQL));
    }

    // Save WuHun data
    std::string wuHunStr;
    {
        std::stringstream ss;
        for (int i = 0; i < 5; ++i)
        {
            if (i > 0) ss << "|";
            for (int j = 0; j < 16; ++j)
            {
                if (j > 0) ss << ":";
                ss << m_WuHun[i][j];
            }
        }
        wuHunStr = ss.str();
    }

    bzero(szSQL, 0x1000u);
    snprintf(
        (char *)szSQL,
        0xFFFu,
        "INSERT INTO `mem_wu_hun` (`cid`,`refresh_times`,`last_refresh_time`,`wu_hun`) VALUES (%lld,%d,%d,'%s') "
        "ON DUPLICATE KEY UPDATE `refresh_times`=%d,`last_refresh_time`=%d,`wu_hun`='%s'",
        nCid,
        m_RefreshTimes,
        m_LastRefreshTime,
        wuHunStr.c_str(),
        m_RefreshTimes,
        m_LastRefreshTime,
        wuHunStr.c_str());

    sqls->push_back(std::string((char*)szSQL));
}

bool WuHunShopDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    // Load wu hun shop items
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_wu_hun_shop` WHERE `cid`=%lld", nCid);

    {
        Answer::MySqlQuery result(db->query((const char *)szSQL));
        while (!result.eof())
        {
            WuHunShop shop;
            memset(&shop, 0, sizeof(shop));
            int32_t ShopId = result.getIntValue("shopid", 0);

            std::string strnIndexs = result.getStringValue("item_index", "");
            std::string strStates = result.getStringValue("item_state", "");

            std::string delims = ":";
            StringVector vnIndexs;
            StringVector vStates;
            StringUtility::split(vnIndexs, strnIndexs, delims);
            StringUtility::split(vStates, strStates, delims);

            for (size_t i = 0; i < vnIndexs.size() && i < vStates.size() && i < 4; ++i)
            {
                shop.nIndex[i] = atoi(vnIndexs[i].c_str());
                shop.nState[i] = (int8_t)atoi(vStates[i].c_str());
            }

            m_WuHunShopMap[ShopId] = shop;
            result.nextRow();
        }
    }

    // Load wu hun data
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_wu_hun` WHERE `cid`=%lld", nCid);

    {
        Answer::MySqlQuery result(db->query((const char *)szSQL));
        while (!result.eof())
        {
            m_RefreshTimes = result.getIntValue("refresh_times", 0);
            m_LastRefreshTime = result.getIntValue("last_refresh_time", 0);

            std::string WuHunString = result.getStringValue("wu_hun", "");
            ParesWuHun(&WuHunString);
            result.nextRow();
        }
    }

    return true;
}

void WuHunShopDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt32(packet, m_RefreshTimes);
    Answer::NetPacket::writeInt32(packet, m_LastRefreshTime);

    Answer::NetPacket::writeInt32(packet, (int32_t)m_WuHunShopMap.size());
    for (const auto& [shopId, shop] : m_WuHunShopMap)
    {
        Answer::NetPacket::writeInt32(packet, shopId);
        for (int i = 0; i < 4; ++i)
        {
            Answer::NetPacket::writeInt32(packet, shop.nIndex[i]);
            Answer::NetPacket::writeInt8(packet, shop.nState[i]);
        }
    }

    for (int j = 0; j < 5; ++j)
        for (int k = 0; k < 16; ++k)
            Answer::NetPacket::writeInt32(packet, m_WuHun[j][k]);
}

void WuHunShopDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{
    if (!inPacket) return;

    m_RefreshTimes = Answer::NetPacket::readInt32(inPacket);
    m_LastRefreshTime = Answer::NetPacket::readInt32(inPacket);

    int32_t nSize = Answer::NetPacket::readInt32(inPacket);
    for (int i = 0; i < nSize; ++i)
    {
        WuHunShop shop;
        memset(&shop, 0, sizeof(shop));
        int32_t ShopId = Answer::NetPacket::readInt32(inPacket);
        for (int j = 0; j < 4; ++j)
        {
            shop.nIndex[j] = Answer::NetPacket::readInt32(inPacket);
            shop.nState[j] = Answer::NetPacket::readInt8(inPacket);
        }
        m_WuHunShopMap[ShopId] = shop;
    }

    for (int j = 0; j < 5; ++j)
        for (int k = 0; k < 16; ++k)
            m_WuHun[j][k] = Answer::NetPacket::readInt32(inPacket);
}

void WuHunShopDBData::ParesWuHun(std::string *p_WuHunString)
{
    if (!p_WuHunString || p_WuHunString->empty()) return;

    std::string delims = "|";
    StringVector vWuHunType;
    StringUtility::split(vWuHunType, *p_WuHunString, delims);

    for (size_t index = 0; index < vWuHunType.size() && index < 5; ++index)
    {
        std::string subDelims = ":";
        StringVector vWuHun;
        StringUtility::split(vWuHun, vWuHunType[index], subDelims);
        for (size_t iSlot = 0; iSlot < vWuHun.size() && iSlot < 16; ++iSlot)
        {
            m_WuHun[index][iSlot] = atoi(vWuHun[iSlot].c_str());
        }
    }
}
