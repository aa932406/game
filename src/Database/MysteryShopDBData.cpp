#include "Database/MysteryShopDBData.h"

MysteryShopDBData::MysteryShopDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4F30; */
    this->lstShop.list();
    /* base init: MysteryShopDBData::CleanUp(this); */
}

MysteryShopDBData::~MysteryShopDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4F30; */
    std::list<MysteryShop>::~list(&this->lstShop);
}

void MysteryShopDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{


    const char *v7; // r12
    int32_t nAutoRefreshTime; // r15d
    int32_t nBuyFlag; // r14d
    const char *v10; // rbx


    _BYTE v16[16]; // [rsp+60h] [rbp-1E0h] BYREF

    std::string strGoods; // [rsp+1D0h] [rbp-70h] BYREF
    std::_List_iterator<MysteryShop> iter; // [rsp+1E0h] [rbp-60h] BYREF

    std::string __x; // [rsp+200h] [rbp-40h] BYREF


    for ( iter = this->lstShop.begin();
    ;
    std::_List_iterator<MysteryShop>::operator++(&iter) )
    {
    v20 = this->lstShop.end();
    if ( !std::_List_iterator<MysteryShop>::operator!=(&iter, &v20) )
    break;
    v4 = std::operator|(std::_Ios_Openmode::_S_out, std::_Ios_Openmode::_S_in);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::basic_stringstream(v16, (unsigned int)v4);
    for ( i = 0; i <= 3; ++i )
    {
    v5 = std::_List_iterator<MysteryShop>::operator->(&iter);
    v6 = std::ostream::operator<<(&v17, (unsigned int)v5->vGoods[i]);
    std::operator<<<std::char_traits<char>>(v6, ":");
    }
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::str(&strGoods, v16);
    bzero(szSQL, 0x1000u);
    v7 = (const char *)std::string::c_str(&strGoods);
    nAutoRefreshTime = std::_List_iterator<MysteryShop>::operator->(&iter)->nAutoRefreshTime;
    nBuyFlag = std::_List_iterator<MysteryShop>::operator->(&iter)->nBuyFlag;
    v10 = (const char *)std::string::c_str(&strGoods);
    v11 = std::_List_iterator<MysteryShop>::operator->(&iter)->nAutoRefreshTime;
    v13 = std::_List_iterator<MysteryShop>::operator->(&iter)->nBuyFlag;
    v12 = std::_List_iterator<MysteryShop>::operator->(&iter);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_char_mystery_shop` (`cid`,`type`,`buy_flag`,`refresh_time`,`goods`) VALUES (%lld,%d,%d,%d,'%s') O"
    "N DUPLICATE KEY UPDATE `buy_flag`=%d,`refresh_time`=%d,`goods`='%s'",
    nCid,
    v12->nType,
    v13,
    v11,
    v10,
    nBuyFlag,
    nAutoRefreshTime,
    v7);

    std::string::string(&__x, (char *)szSQL, &v22);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(&v22);
    std::string::~string(&strGoods);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::~basic_stringstream(v16);
    }
}

bool MysteryShopDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    const char *StringValue; // rax


    const char *v10; // rax

    StringVector vGoods; // [rsp+30h] [rbp-90h] BYREF
    std::string strGoods; // [rsp+50h] [rbp-70h] BYREF
    MysteryShop shop; // [rsp+60h] [rbp-60h] BYREF
    Answer::MySqlQuery result; // [rsp+80h] [rbp-40h] BYREF

    std::string delims; // [rsp+A0h] [rbp-20h] BYREF


    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_mystery_shop` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    while ( !Answer::MySqlQuery::eof(&result) )
    {
    memset(&shop, 0, sizeof(shop));
    shop.nType = Answer::MySqlQuery::getIntValue(&result, "type", 0);
    shop.nBuyFlag = Answer::MySqlQuery::getIntValue(&result, "buy_flag", 0);
    shop.nAutoRefreshTime = Answer::MySqlQuery::getIntValue(&result, "refresh_time", 0);

    StringValue = Answer::MySqlQuery::getStringValue(&result, "goods", byte_8CFE00);
    std::string::string(&strGoods, StringValue, &v20);
    std::allocator<char>::~allocator(&v20);

    std::string::string(&delims, ":", &v22);
    Answer::StringUtility::split(&vGoods, &strGoods, &delims, 0);
    std::string::~string(&delims);
    std::allocator<char>::~allocator(&v22);
    for ( i = 0; i <= 3; ++i )
    {
    v11 = i;
    if ( v11 >= vGoods.size() )
    break;
    v8 = i;
    v9 = std::vector<std::string>::operator[](&vGoods, i);
    v10 = (const char *)std::string::c_str(v9);
    shop.vGoods[v8] = atoi(v10);
    }
    this->lstShop.push_back(&shop);
    Answer::MySqlQuery::nextRow(&result);
    std::vector<std::string>::~vector(&vGoods);
    std::string::~string(&strGoods);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return 0;
}

void MysteryShopDBData::PackageData(Answer::NetPacket *packet)
{

    int8_t nType; // dl
    int32_t nBuyFlag; // edx
    int32_t nAutoRefreshTime; // edx

    std::_List_iterator<MysteryShop> iter; // [rsp+10h] [rbp-20h] BYREF
    std::_List_iterator<MysteryShop> __x; // [rsp+20h] [rbp-10h] BYREF
    int32_t i; // [rsp+2Ch] [rbp-4h]

    if ( packet )
    {
    v2 = this->lstShop.size();
    Answer::NetPacket::writeInt32(packet, v2);
    for ( iter = this->lstShop.begin();
    ;
    std::_List_iterator<MysteryShop>::operator++(&iter) )
    {
    __x = this->lstShop.end();
    if ( !std::_List_iterator<MysteryShop>::operator!=(&iter, &__x) )
    break;
    nType = std::_List_iterator<MysteryShop>::operator->(&iter)->nType;
    Answer::NetPacket::writeInt8(packet, nType);
    nBuyFlag = std::_List_iterator<MysteryShop>::operator->(&iter)->nBuyFlag;
    Answer::NetPacket::writeInt32(packet, nBuyFlag);
    nAutoRefreshTime = std::_List_iterator<MysteryShop>::operator->(&iter)->nAutoRefreshTime;
    Answer::NetPacket::writeInt32(packet, nAutoRefreshTime);
    for ( i = 0; i <= 3; ++i )
    {
    v6 = std::_List_iterator<MysteryShop>::operator->(&iter);
    Answer::NetPacket::writeInt32(packet, v6->vGoods[i]);
    }
    }
    }
}

（内容由AI生成，仅供参考）
