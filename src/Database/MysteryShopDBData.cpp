#include "Database/MysteryShopDBData.h"
#include "Answer/MySqlQuery.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/StringUtility.h"
#include "Answer/NetPacket.h"

MysteryShopDBData::MysteryShopDBData()
{
}

MysteryShopDBData::~MysteryShopDBData()
{
}

void MysteryShopDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    for (const auto& shop : lstShop)
    {
        bzero(szSQL, 0x1000u);
        snprintf(
            (char *)szSQL,
            0xFFFu,
            "INSERT INTO `mem_char_mystery_shop` (`cid`,`type`,`buy_flag`,`refresh_time`,`goods`) VALUES (%lld,%d,%d,%d,'%d:%d:%d:%d') "
            "ON DUPLICATE KEY UPDATE `buy_flag`=%d,`refresh_time`=%d,`goods`='%d:%d:%d:%d'",
            nCid,
            shop.nType,
            shop.nBuyFlag,
            shop.nAutoRefreshTime,
            shop.vGoods[0], shop.vGoods[1], shop.vGoods[2], shop.vGoods[3],
            shop.nBuyFlag,
            shop.nAutoRefreshTime,
            shop.vGoods[0], shop.vGoods[1], shop.vGoods[2], shop.vGoods[3]);

        sqls->push_back(std::string((char*)szSQL));
    }
}

bool MysteryShopDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_mystery_shop` WHERE `cid`=%lld", nCid);

    Answer::MySqlQuery result(db->query((const char *)szSQL));
    while (!result.eof())
    {
        MysteryShop shop;
        memset(&shop, 0, sizeof(shop));
        shop.nType = result.getIntValue("type", 0);
        shop.nBuyFlag = result.getIntValue("buy_flag", 0);
        shop.nAutoRefreshTime = result.getIntValue("refresh_time", 0);

        std::string strGoods = result.getStringValue("goods", "");
        std::string delims = ":";
        StringVector vGoods;
        StringUtility::split(vGoods, strGoods, delims);
        for (size_t i = 0; i < vGoods.size() && i < 4; ++i)
        {
            shop.vGoods[i] = atoi(vGoods[i].c_str());
        }

        lstShop.push_back(shop);
        result.nextRow();
    }
    return false;
}

void MysteryShopDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt32(packet, (int32_t)lstShop.size());
    for (const auto& shop : lstShop)
    {
        Answer::NetPacket::writeInt8(packet, shop.nType);
        Answer::NetPacket::writeInt32(packet, shop.nBuyFlag);
        Answer::NetPacket::writeInt32(packet, shop.nAutoRefreshTime);
        for (int i = 0; i < 4; ++i)
            Answer::NetPacket::writeInt32(packet, shop.vGoods[i]);
    }
}
