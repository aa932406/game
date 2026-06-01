#include "Database/WuHunShopDBData.h"

WuHunShopDBData::WuHunShopDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4EF0; */
    this->m_WuHunShopMap.map();
    /* base init: WuHunShopDBData::CleanUp(this); */
}

WuHunShopDBData::~WuHunShopDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4EF0; */
    std::map<int,WuHunShop>::~map(&this->m_WuHunShopMap);
}

void WuHunShopDBData::CleanUp()
{
    int32_t i; // [rsp+18h] [rbp-8h]
    int32_t j; // [rsp+1Ch] [rbp-4h]

    this->m_WuHunShopMap.clear();
    this->m_RefreshTimes = 5;
    this->m_LastRefreshTime = 0;
    for ( i = 0; i <= 4; ++i )
    {
    for ( j = 0; j <= 15; ++j )
    this->m_WuHun[i][j] = 0;
    }
}

void WuHunShopDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{


    std::pair<const int,WuHunShop> *v6; // rax

    std::pair<const int,WuHunShop> *v8; // rax

    const char *v10; // r13
    const char *v11; // r12
    const char *v12; // rbx
    const char *v13; // r14
    std::pair<const int,WuHunShop> *v14; // rax
    const char *v15; // rbx
    int32_t m_LastRefreshTime; // r13d
    int32_t m_RefreshTimes; // r12d
    const char *v18; // rax
    _BYTE v21[16]; // [rsp+40h] [rbp-380h] BYREF

    _BYTE v23[16]; // [rsp+1B0h] [rbp-210h] BYREF

    std::string strStates; // [rsp+320h] [rbp-A0h] BYREF
    std::string strnIndexs; // [rsp+330h] [rbp-90h] BYREF
    std::_Rb_tree_iterator<std::pair<const int,WuHunShop> > iter; // [rsp+340h] [rbp-80h] BYREF
    std::_Rb_tree_iterator<std::pair<const int,WuHunShop> > v28; // [rsp+350h] [rbp-70h] BYREF
    std::string __x; // [rsp+360h] [rbp-60h] BYREF


    for ( iter = this->m_WuHunShopMap.begin();
    ;
    std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator++(&iter) )
    {
    v28 = this->m_WuHunShopMap.end();
    if ( !std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator!=(&iter, &v28) )
    break;
    v4 = std::operator|(std::_Ios_Openmode::_S_out, std::_Ios_Openmode::_S_in);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::basic_stringstream(v23, (unsigned int)v4);
    v5 = std::operator|(std::_Ios_Openmode::_S_out, std::_Ios_Openmode::_S_in);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::basic_stringstream(v21, (unsigned int)v5);
    for ( i = 0; i <= 3; ++i )
    {
    v6 = std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator->(&iter);
    v7 = std::ostream::operator<<(&v24, (unsigned int)v6->second.nIndex[i]);
    std::operator<<<std::char_traits<char>>(v7, ":");
    v8 = std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator->(&iter);
    v9 = std::ostream::operator<<(&v22, (unsigned int)v8->second.nState[i]);
    std::operator<<<std::char_traits<char>>(v9, ":");
    }
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::str(&strnIndexs, v23);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::str(&strStates, v21);
    bzero(szSQL, 0x1000u);
    v10 = (const char *)std::string::c_str(&strStates);
    v11 = (const char *)std::string::c_str(&strnIndexs);
    v12 = (const char *)std::string::c_str(&strStates);
    v13 = (const char *)std::string::c_str(&strnIndexs);
    v14 = std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator->(&iter);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_wu_hun_shop` (`cid`,`shopid`,`item_index`,`item_state`) VALUES (%lld,%d,'%s','%s') ON DUPLICATE K"
    "EY UPDATE `item_index`='%s',`item_state`='%s'",
    nCid,
    v14->first,
    v13,
    v12,
    v11,
    v10);

    std::string::string(&__x, (char *)szSQL, &v30);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(&v30);
    std::string::~string(&strStates);
    std::string::~string(&strnIndexs);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::~basic_stringstream(v21);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::~basic_stringstream(v23);
    }
    bzero(szSQL, 0x1000u);
    WuHunShopDBData::GetWuHunString((WuHunShopDBData *const)&v31);
    v15 = (const char *)std::string::c_str(&v31);
    m_LastRefreshTime = this->m_LastRefreshTime;
    m_RefreshTimes = this->m_RefreshTimes;
    WuHunShopDBData::GetWuHunString((WuHunShopDBData *const)&v32);
    v18 = (const char *)std::string::c_str(&v32);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_wu_hun` (`cid`,`refresh_times`,`last_refresh_time`,`wu_hun`) VALUES (%lld,%d,%d,'%s') ON DUPLICATE "
    "KEY UPDATE `refresh_times`=%d,`last_refresh_time`=%d,`wu_hun`='%s'",
    nCid,
    this->m_RefreshTimes,
    this->m_LastRefreshTime,
    v18,
    m_RefreshTimes,
    m_LastRefreshTime,
    v15);
    std::string::~string(&v32);
    std::string::~string(&v31);

    std::string::string(&v33, (char *)szSQL, &v34);
    std::list<std::string>::push_back(sqls, &v33);
    std::string::~string(&v33);
    std::allocator<char>::~allocator(&v34);
}

bool WuHunShopDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    const char *StringValue; // rax
    const char *v8; // rax


    const char *v11; // rax


    const char *v14; // rax


    const char *v20; // rax
    std::string WuHunString; // [rsp+30h] [rbp-130h] BYREF
    WuHunShop shop_0; // [rsp+40h] [rbp-120h] BYREF
    Answer::MySqlQuery result_0; // [rsp+60h] [rbp-100h] BYREF
    StringVector vStates; // [rsp+80h] [rbp-E0h] BYREF
    StringVector vnIndexs; // [rsp+A0h] [rbp-C0h] BYREF
    std::string strStates; // [rsp+C0h] [rbp-A0h] BYREF
    std::string strnIndexs; // [rsp+D0h] [rbp-90h] BYREF
    int32_t ShopId; // [rsp+DCh] [rbp-84h] BYREF
    WuHunShop shop; // [rsp+E0h] [rbp-80h] BYREF
    Answer::MySqlQuery result; // [rsp+100h] [rbp-60h] BYREF


    std::string delims; // [rsp+120h] [rbp-40h] BYREF


    std::string p_WuHunString; // [rsp+140h] [rbp-20h] BYREF


    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_wu_hun_shop` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    while ( !Answer::MySqlQuery::eof(&result) )
    {
    memset(&shop, 0, sizeof(shop));
    ShopId = Answer::MySqlQuery::getIntValue(&result, "shopid", 0);

    StringValue = Answer::MySqlQuery::getStringValue(&result, "item_index", byte_8CFE00);
    std::string::string(&strnIndexs, StringValue, &v34);
    std::allocator<char>::~allocator(&v34);

    v8 = Answer::MySqlQuery::getStringValue(&result, "item_state", byte_8CFE00);
    std::string::string(&strStates, v8, &v35);
    std::allocator<char>::~allocator(&v35);

    std::string::string(&delims, ":", &v37);
    Answer::StringUtility::split(&vnIndexs, &strnIndexs, &delims, 0);
    std::string::~string(&delims);
    std::allocator<char>::~allocator(&v37);

    std::string::string(&v38, ":", &v39);
    Answer::StringUtility::split(&vStates, &strStates, &v38, 0);
    std::string::~string(&v38);
    std::allocator<char>::~allocator(&v39);
    for ( i = 0; i <= 3; ++i )
    {
    v15 = i;
    if ( v15 >= vnIndexs.size() )
    break;
    v16 = i;
    if ( v16 >= vStates.size() )
    break;
    v9 = i;
    v10 = std::vector<std::string>::operator[](&vnIndexs, i);
    v11 = (const char *)std::string::c_str(v10);
    shop.nIndex[v9] = atoi(v11);
    v12 = i;
    v13 = std::vector<std::string>::operator[](&vStates, i);
    v14 = (const char *)std::string::c_str(v13);
    shop.nState[v12] = atoi(v14);
    }
    v18 = std::map<int,WuHunShop>::operator[](&this->m_WuHunShopMap, &ShopId);
    *v18 = shop;
    Answer::MySqlQuery::nextRow(&result);
    std::vector<std::string>::~vector(&vStates);
    std::vector<std::string>::~vector(&vnIndexs);
    std::string::~string(&strStates);
    std::string::~string(&strnIndexs);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_wu_hun` WHERE `cid`=%lld", nCid);
    v19 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result_0, v19);
    while ( !Answer::MySqlQuery::eof(&result_0) )
    {
    memset(&shop_0, 0, sizeof(shop_0));
    this->m_RefreshTimes = Answer::MySqlQuery::getIntValue(&result_0, "refresh_times", 0);
    this->m_LastRefreshTime = Answer::MySqlQuery::getIntValue(&result_0, "last_refresh_time", 0);

    v20 = Answer::MySqlQuery::getStringValue(&result_0, "wu_hun", byte_8CFE00);
    std::string::string(&WuHunString, v20, &v40);
    std::allocator<char>::~allocator(&v40);
    std::string::string(&p_WuHunString, &WuHunString);
    this->ParesWuHun(&p_WuHunString);
    std::string::~string(&p_WuHunString);
    Answer::MySqlQuery::nextRow(&result_0);
    std::string::~string(&WuHunString);
    }
    Answer::MySqlQuery::~MySqlQuery(&result_0);
    return 1;
}

void WuHunShopDBData::PackageData(Answer::NetPacket *packet)
{

    int first; // edx
    std::pair<const int,WuHunShop> *v4; // rax
    std::pair<const int,WuHunShop> *v5; // rax
    std::_Rb_tree_iterator<std::pair<const int,WuHunShop> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const int,WuHunShop> > __x; // [rsp+20h] [rbp-20h] BYREF
    int32_t i; // [rsp+34h] [rbp-Ch]
    int32_t j; // [rsp+38h] [rbp-8h]
    int32_t k; // [rsp+3Ch] [rbp-4h]

    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, this->m_RefreshTimes);
    Answer::NetPacket::writeInt32(packet, this->m_LastRefreshTime);
    v2 = this->m_WuHunShopMap.size();
    Answer::NetPacket::writeInt32(packet, v2);
    for ( iter = this->m_WuHunShopMap.begin();
    ;
    std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator++(&iter) )
    {
    __x = this->m_WuHunShopMap.end();
    if ( !std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator!=(&iter, &__x) )
    break;
    first = std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator->(&iter)->first;
    Answer::NetPacket::writeInt32(packet, first);
    for ( i = 0; i <= 3; ++i )
    {
    v4 = std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator->(&iter);
    Answer::NetPacket::writeInt32(packet, v4->second.nIndex[i]);
    v5 = std::_Rb_tree_iterator<std::pair<int const,WuHunShop>>::operator->(&iter);
    Answer::NetPacket::writeInt8(packet, v5->second.nState[i]);
    }
    }
    for ( j = 0; j <= 4; ++j )
    {
    for ( k = 0; k <= 15; ++k )
    Answer::NetPacket::writeInt32(packet, this->m_WuHun[j][k]);
    }
    }
}

void WuHunShopDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{


    WuHunShop shop; // [rsp+20h] [rbp-40h] BYREF
    int32_t ShopId; // [rsp+38h] [rbp-28h] BYREF
    int32_t nSize; // [rsp+3Ch] [rbp-24h]
    int32_t i; // [rsp+40h] [rbp-20h]
    int32_t j; // [rsp+44h] [rbp-1Ch]
    int32_t j_0; // [rsp+48h] [rbp-18h]
    int32_t k; // [rsp+4Ch] [rbp-14h]

    if ( inPacket )
    {
    this->m_RefreshTimes = Answer::NetPacket::readInt32(inPacket);
    this->m_LastRefreshTime = Answer::NetPacket::readInt32(inPacket);
    nSize = Answer::NetPacket::readInt32(inPacket);
    for ( i = 0; i < nSize; ++i )
    {
    ShopId = Answer::NetPacket::readInt32(inPacket);
    memset(&shop, 0, sizeof(shop));
    for ( j = 0; j <= 3; ++j )
    {
    v3 = j;
    shop.nIndex[v3] = Answer::NetPacket::readInt32(inPacket);
    v4 = j;
    shop.nState[v4] = Answer::NetPacket::readInt8(inPacket);
    }
    v5 = std::map<int,WuHunShop>::operator[](&this->m_WuHunShopMap, &ShopId);
    *v5 = shop;
    }
    for ( j_0 = 0; j_0 <= 4; ++j_0 )
    {
    for ( k = 0; k <= 15; ++k )
    {
    v6 = j_0;
    v7 = k;
    this->m_WuHun[v6][v7] = Answer::NetPacket::readInt32(inPacket);
    }
    }
    }
}

void WuHunShopDBData::ParesWuHun(std::string *p_WuHunString)
{


    const char *v6; // rax


    StringVector vWuHun; // [rsp+10h] [rbp-80h] BYREF
    StringVector vWuHunType; // [rsp+30h] [rbp-60h] BYREF
    std::string delims; // [rsp+50h] [rbp-40h] BYREF


    uint32_t iSlot; // [rsp+7Ch] [rbp-14h]

    if ( !(unsigned __int8)std::string::empty(p_WuHunString) )
    {

    std::string::string(&delims, "|", &v14);
    Answer::StringUtility::split(&vWuHunType, p_WuHunString, &delims, 0);
    std::string::~string(&delims);
    std::allocator<char>::~allocator(&v14);
    for ( index = 0; ; ++index )
    {
    v9 = index;
    if ( v9 >= vWuHunType.size() || index > 4 )
    break;

    std::string::string(&v15, ":", &v16);
    v2 = std::vector<std::string>::operator[](&vWuHunType, index);
    Answer::StringUtility::split(&vWuHun, v2, &v15, 0);
    std::string::~string(&v15);
    std::allocator<char>::~allocator(&v16);
    for ( iSlot = 0; ; ++iSlot )
    {
    v7 = iSlot;
    if ( v7 >= vWuHun.size() || iSlot > 0xF )
    break;
    v3 = index;
    v4 = iSlot;
    v5 = std::vector<std::string>::operator[](&vWuHun, iSlot);
    v6 = (const char *)std::string::c_str(v5);
    this->m_WuHun[v3][v4] = atoi(v6);
    }
    std::vector<std::string>::~vector(&vWuHun);
    }
    std::vector<std::string>::~vector(&vWuHunType);
    }
}

（内容由AI生成，仅供参考）
