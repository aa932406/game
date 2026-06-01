#include "Database/ExchangeDBData.h"

ExchangeDBData::ExchangeDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4E30; */
    this->lstExchange.list();
    /* base init: ExchangeDBData::CleanUp(this); */
}

ExchangeDBData::~ExchangeDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4E30; */
    std::list<ExchangeRecord>::~list(&this->lstExchange);
}

void ExchangeDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    int32_t nCount; // ebx
    int nIndex; // r12d

    std::_List_iterator<ExchangeRecord> iter; // [rsp+30h] [rbp-50h] BYREF
    std::string __x; // [rsp+40h] [rbp-40h] BYREF


    _BYTE v14[17]; // [rsp+6Fh] [rbp-11h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "DELETE FROM `mem_char_exchange` WHERE `cid`=%lld", nCid);

    std::string::string(&__x, (char *)szSQL, &v11);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(&v11);
    for ( iter = this->lstExchange.begin();
    ;
    std::_List_iterator<ExchangeRecord>::operator++(&iter) )
    {
    v12 = this->lstExchange.end();
    if ( !std::_List_iterator<ExchangeRecord>::operator!=(&iter, &v12) )
    break;
    bzero(szSQL, 0x1000u);
    nCount = std::_List_iterator<ExchangeRecord>::operator->(&iter)->nCount;
    nIndex = std::_List_iterator<ExchangeRecord>::operator->(&iter)->nIndex;
    v6 = std::_List_iterator<ExchangeRecord>::operator->(&iter);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_char_exchange` (`cid`,`type`,`index`,`count`) VALUES (%lld,%d,%d,%d)",
    nCid,
    v6->nType,
    nIndex,
    nCount);
    std::allocator<char>::allocator(v14);
    std::string::string(&v13, (char *)szSQL, v14);
    std::list<std::string>::push_back(sqls, &v13);
    std::string::~string(&v13);
    std::allocator<char>::~allocator(v14);
    }
}

bool ExchangeDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    ExchangeRecord shop; // [rsp+30h] [rbp-40h] BYREF
    Answer::MySqlQuery result; // [rsp+40h] [rbp-30h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_exchange` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    while ( !Answer::MySqlQuery::eof(&result) )
    {
    shop = 0;
    shop.nType = Answer::MySqlQuery::getIntValue(&result, "type", 0);
    shop.nIndex = Answer::MySqlQuery::getIntValue(&result, "index", 0);
    shop.nCount = Answer::MySqlQuery::getIntValue(&result, "count", 0);
    this->lstExchange.push_back(&shop);
    Answer::MySqlQuery::nextRow(&result);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return 0;
}

void ExchangeDBData::PackageData(Answer::NetPacket *packet)
{

    int8_t nType; // dl
    int16_t nIndex; // dx
    int32_t nCount; // edx
    std::_List_iterator<ExchangeRecord> iter; // [rsp+10h] [rbp-20h] BYREF
    std::_List_iterator<ExchangeRecord> __x; // [rsp+20h] [rbp-10h] BYREF

    if ( packet )
    {
    v2 = this->lstExchange.size();
    Answer::NetPacket::writeInt32(packet, v2);
    for ( iter = this->lstExchange.begin();
    ;
    std::_List_iterator<ExchangeRecord>::operator++(&iter) )
    {
    __x = this->lstExchange.end();
    if ( !std::_List_iterator<ExchangeRecord>::operator!=(&iter, &__x) )
    break;
    nType = std::_List_iterator<ExchangeRecord>::operator->(&iter)->nType;
    Answer::NetPacket::writeInt8(packet, nType);
    nIndex = std::_List_iterator<ExchangeRecord>::operator->(&iter)->nIndex;
    Answer::NetPacket::writeInt16(packet, nIndex);
    nCount = std::_List_iterator<ExchangeRecord>::operator->(&iter)->nCount;
    Answer::NetPacket::writeInt32(packet, nCount);
    }
    }
}

（内容由AI生成，仅供参考）
