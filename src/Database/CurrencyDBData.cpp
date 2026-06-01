#include "Database/CurrencyDBData.h"

CurrencyDBData::CurrencyDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4FF0; */
    /* base init: CurrencyDBData::CleanUp(this); */
}

CurrencyDBData::~CurrencyDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4FF0; */
}

void CurrencyDBData::CleanUp()
{
    bzero(this->vCurrency, 0x60u);
}

void CurrencyDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    std::string __x; // [rsp+130h] [rbp-40h] BYREF
    _BYTE v7[49]; // [rsp+13Fh] [rbp-31h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT `mem_char_currency`(`cid`,`money`,`gold`,`cash`,`vigour`,`draw_score`,`boss_score`,`bind_money`,`honor`,`ac_s"
    "core`,`contribution`,`blood`,`dust`) VALUES (%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld) ON DU"
    "PLICATE KEY UPDATE `money`=%lld,`gold`=%lld,`cash`=%lld,`vigour`=%lld,`draw_score`=%lld,`boss_score`=%lld,`bind_mone"
    "y`=%lld,`honor`=%lld,`ac_score`=%lld,`contribution`=%lld,`blood`=%lld,`dust`=%lld",
    nCid,
    this->vCurrency[0],
    this->vCurrency[1],
    this->vCurrency[2],
    this->vCurrency[3],
    this->vCurrency[4],
    this->vCurrency[5],
    this->vCurrency[6],
    this->vCurrency[7],
    this->vCurrency[8],
    this->vCurrency[9],
    this->vCurrency[10],
    this->vCurrency[11],
    this->vCurrency[0],
    this->vCurrency[1],
    this->vCurrency[2],
    this->vCurrency[3],
    this->vCurrency[4],
    this->vCurrency[5],
    this->vCurrency[6],
    this->vCurrency[7],
    this->vCurrency[8],
    this->vCurrency[9],
    this->vCurrency[10],
    this->vCurrency[11]);
    std::allocator<char>::allocator(v7);
    std::string::string(&__x, (char *)szSQL, v7);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(v7);
}

bool CurrencyDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{


    Answer::MySqlQuery result; // [rsp+30h] [rbp-30h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_currency` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    if ( !Answer::MySqlQuery::eof(&result) )
    {
    this->vCurrency[0] = Answer::MySqlQuery::getInt64Value(&result, "money", 0);
    this->vCurrency[1] = Answer::MySqlQuery::getInt64Value(&result, "gold", 0);
    this->vCurrency[2] = Answer::MySqlQuery::getInt64Value(&result, "cash", 0);
    this->vCurrency[3] = Answer::MySqlQuery::getInt64Value(&result, "vigour", 0);
    this->vCurrency[4] = Answer::MySqlQuery::getInt64Value(&result, "draw_score", 0);
    this->vCurrency[5] = Answer::MySqlQuery::getInt64Value(&result, "boss_score", 0);
    this->vCurrency[6] = Answer::MySqlQuery::getInt64Value(&result, "bind_money", 0);
    this->vCurrency[7] = Answer::MySqlQuery::getInt64Value(&result, "honor", 0);
    this->vCurrency[8] = Answer::MySqlQuery::getInt64Value(&result, "ac_score", 0);
    this->vCurrency[9] = Answer::MySqlQuery::getInt64Value(&result, "contribution", 0);
    this->vCurrency[10] = Answer::MySqlQuery::getInt64Value(&result, "blood", 0);
    this->vCurrency[11] = Answer::MySqlQuery::getInt64Value(&result, "dust", 0);
    v7 = 1;
    }
    else
    {
    v7 = 0;
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return v7;
}

void CurrencyDBData::PackageData(Answer::NetPacket *packet)
{
    int32_t i; // [rsp+1Ch] [rbp-4h]

    for ( i = 0; i <= 11; ++i )
    Answer::NetPacket::writeInt64(packet, this->vCurrency[i]);
}

（内容由AI生成，仅供参考）
