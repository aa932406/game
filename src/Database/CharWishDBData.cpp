#include "Database/CharWishDBData.h"

CharWishDBData::CharWishDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4E70; */
    this->lstWishs.list();
    /* base init: CharWishDBData::CleanUp(this); */
}

CharWishDBData::~CharWishDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4E70; */
    std::list<CharWishInfo>::~list(&this->lstWishs);
}

void CharWishDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{

    const char *v5; // r12
    int32_t nItemId; // r15d

    const char *v8; // rbx


    int nGetReward; // [rsp+44h] [rbp-8Ch]
    int32_t nStartTime; // [rsp+48h] [rbp-88h]

    std::_List_iterator<CharWishInfo> iter; // [rsp+70h] [rbp-60h] BYREF

    std::string __x; // [rsp+90h] [rbp-40h] BYREF
    _BYTE v20[49]; // [rsp+9Fh] [rbp-31h] BYREF

    for ( iter = this->lstWishs.begin();
    ;
    std::_List_iterator<CharWishInfo>::operator++(&iter) )
    {
    v18 = this->lstWishs.end();
    if ( !std::_List_iterator<CharWishInfo>::operator!=(&iter, &v18) )
    break;
    bzero(szSQL, 0x1000u);
    v4 = std::_List_iterator<CharWishInfo>::operator->(&iter);
    v5 = (const char *)std::string::c_str(&v4->strInfo);
    nGetReward = std::_List_iterator<CharWishInfo>::operator->(&iter)->nGetReward;
    nStartTime = std::_List_iterator<CharWishInfo>::operator->(&iter)->nStartTime;
    nItemId = std::_List_iterator<CharWishInfo>::operator->(&iter)->nItemId;
    v7 = std::_List_iterator<CharWishInfo>::operator->(&iter);
    v8 = (const char *)std::string::c_str(&v7->strInfo);
    v9 = std::_List_iterator<CharWishInfo>::operator->(&iter)->nGetReward;
    v10 = std::_List_iterator<CharWishInfo>::operator->(&iter)->nStartTime;
    v14 = std::_List_iterator<CharWishInfo>::operator->(&iter)->nItemId;
    v11 = std::_List_iterator<CharWishInfo>::operator->(&iter);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_char_wish` (`cid`,`id`,`item_id`,`start_time`,`flag`,`info`) VALUES (%lld,%d,%d,%d,%d,'%s') ON DU"
    "PLICATE KEY UPDATE `item_id`=%d,`start_time`=%d,`flag`=%d,`info`='%s'",
    nCid,
    v11->nId,
    v14,
    v10,
    v9,
    v8,
    nItemId,
    nStartTime,
    nGetReward,
    v5);
    std::allocator<char>::allocator(v20);
    std::string::string(&__x, (char *)szSQL, v20);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(v20);
    }
}

bool CharWishDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    const char *StringValue; // rax
    CharWishInfo info; // [rsp+30h] [rbp-50h] BYREF
    Answer::MySqlQuery result; // [rsp+50h] [rbp-30h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_wish` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    while ( !Answer::MySqlQuery::eof(&result) )
    {
    CharWishInfo::CharWishInfo(&info);
    info.nId = Answer::MySqlQuery::getIntValue(&result, "id", 0);
    info.nItemId = Answer::MySqlQuery::getIntValue(&result, "item_id", 0);
    info.nStartTime = Answer::MySqlQuery::getIntValue(&result, "start_time", 0);
    info.nGetReward = Answer::MySqlQuery::getIntValue(&result, "flag", 0);
    StringValue = Answer::MySqlQuery::getStringValue(&result, "info", byte_8CFE00);
    std::string::operator=(&info.strInfo, StringValue);
    this->lstWishs.push_back(&info);
    Answer::MySqlQuery::nextRow(&result);
    CharWishInfo::~CharWishInfo(&info);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return 0;
}

void CharWishDBData::PackageData(Answer::NetPacket *packet)
{

    int32_t nId; // edx
    int32_t nItemId; // edx
    int32_t nStartTime; // edx
    int8_t nGetReward; // dl
    std::string *p_strInfo; // rdx
    std::_List_iterator<CharWishInfo> iter; // [rsp+10h] [rbp-20h] BYREF
    std::_List_iterator<CharWishInfo> __x; // [rsp+20h] [rbp-10h] BYREF

    if ( packet )
    {
    v2 = this->lstWishs.size();
    Answer::NetPacket::writeInt32(packet, v2);
    for ( iter = this->lstWishs.begin();
    ;
    std::_List_iterator<CharWishInfo>::operator++(&iter) )
    {
    __x = this->lstWishs.end();
    if ( !std::_List_iterator<CharWishInfo>::operator!=(&iter, &__x) )
    break;
    nId = std::_List_iterator<CharWishInfo>::operator->(&iter)->nId;
    Answer::NetPacket::writeInt32(packet, nId);
    nItemId = std::_List_iterator<CharWishInfo>::operator->(&iter)->nItemId;
    Answer::NetPacket::writeInt32(packet, nItemId);
    nStartTime = std::_List_iterator<CharWishInfo>::operator->(&iter)->nStartTime;
    Answer::NetPacket::writeInt32(packet, nStartTime);
    nGetReward = std::_List_iterator<CharWishInfo>::operator->(&iter)->nGetReward;
    Answer::NetPacket::writeInt8(packet, nGetReward);
    p_strInfo = &std::_List_iterator<CharWishInfo>::operator->(&iter)->strInfo;
    Answer::NetPacket::writeUTF8(packet, p_strInfo);
    }
    }
}

（内容由AI生成，仅供参考）
